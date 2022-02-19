// visSIFT-MiniAX.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <windows.h> // Photron SDK用
#include <PDCLIB.h> // Photron SDK用
#include <direct.h> // ディレクトリ作成用
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <algorithm>
#include <vector>
#include <functional>

using namespace cv;

/* 関数宣言 */
void Init();
PDC_DETECT_NUM_INFO DetectDevice(unsigned long* IPList);
void OpenDevice(PDC_DETECT_NUM_INFO DetectNumInfo);
void SetRecordRate(unsigned long nRate);

void SetShutterSpeed(unsigned long nFps);
cv::Mat GetLiveImage();
void Shading();
void SetStatus();
void showHist(cv::Mat frame_16bit);
String MakeDirWithTime(String dir_path);

/* グローバル変数 */
unsigned long nDeviceNo = 0; /* デバイスナンバー */
static unsigned long nChildNo = 1; /* 子デバイスナンバー */
static unsigned long nWidth = 1024; /* 画像横幅[pixel] */
static unsigned long nHeight = 1024; /* 画像縦幅[pixel] */
static unsigned long fps = 50; /* 撮影のフレームレート[sec] */
static unsigned long shutterSpeed = 50; /* シャッタースピード。1/f[sec] で表すときのfの値を設定 */
int hist_max = 4096;
int hist_min = 0;

int main()
{
    /************************************************************
    *
    *  高速度カメラのセットアップ   Setup of the camera
    * 
    *************************************************************/
    unsigned long IPList[PDC_MAX_DEVICE]; /* 検索対象IPアドレス */
    IPList[0] = 0xC0A8000A; /* C0=16*12=192, A8=16*10+8=168, 00=0, 0A=10 -> 192.168.0.10 */

    printf("シェーディングを行います。レンズにキャップをつけたらエンターキーを押してください。");
    int c = getchar();

    Init(); // 初期化
    PDC_DETECT_NUM_INFO DetectNumInfo = DetectDevice(IPList); // デバイスの検出
    OpenDevice(DetectNumInfo); // デバイスのオープン
    SetRecordRate(fps); // フレームレートを設定
    //SetShutterSpeed(shutterSpeed); // シャッタースピードの設定は実行しない。なぜかシャッタースピードを2フレーム目から1/fに戻ってしまうため。
    Shading(); // シェーディング（レンズをキャップで遮光した状態で行う）
    SetStatus(); // ライブモードに設定する
    
    printf("レンズからキャップを外したらエンターキーを押してください。");
    c = getchar();


	/************************************************************
    *
    *  カメラの初期位置決め   Set first position of the camera
    *
    *************************************************************/
    // カメラ移動のための一時停止
    while (1) {
        cv::Mat frame_16bit = GetLiveImage();
        cv::Mat frame_16bit_normalized = frame_16bit.clone();
        cv::Mat frame;
        showHist(frame_16bit);
        frame_16bit_normalized = frame_16bit - hist_min; // 下限（hist_min）以下の画素値を0にする
        frame_16bit_normalized = frame_16bit_normalized + cv::Scalar(hist_min + (65536 - hist_max)); // 上限（hist_max）以上の画素値をMaxにする
        cv::normalize(frame_16bit_normalized, frame_16bit_normalized, 0, 4096, cv::NORM_MINMAX); // 正規化
        frame_16bit_normalized.convertTo(frame, CV_8U, 1.0 / 16); // 12bit -> 8bit ∴ 1/16
        cv::putText(frame, "Move to 1st position. Then, push 's'.", cv::Point(25, 75), cv::FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(255), 3);
        cv::imshow("view for focus check", frame); // 大体のピントを目視確認するための確認画面
        int k = cv::waitKey(int(1000 / fps));
        if (k == 115) {
            cv::destroyWindow("view for focus check");
            cv::destroyWindow("histgram");
            break;
        }
    }

    /************************************************************
    *
    *  SIFT特徴量マッチング   Matching of SIFT description
    *
    *************************************************************/
    String filepath = MakeDirWithTime("C:/Users/bmpe/repos/visSIFT-MiniAX/Result/"); // 画像保存用フォルダの作成とそのフルパスの取得（フォルダ名は現在時刻）
    cv::Mat prev_frame, best_frame = cv::Mat::zeros(nWidth, nHeight, CV_8U); // フレーム
    cv::Mat prev_frame_keypoints = cv::Mat::zeros(nWidth, nHeight, CV_8UC3); // キーポイント描画したフレーム
    cv::Ptr<Feature2D> detector = cv::SIFT::create(0, 6, 0.04, 10.0, 3.0); // SIFTキーポイント検出器（パラメータは基本OpenMVGと同じ。Edge thresholdだけ厳しめに設定。）
    std::vector<cv::KeyPoint> prev_keypoints, best_keypoints; // キーポイント
    cv::Mat prev_descriptors, best_descriptors; // 特徴量記述子
    cv::Ptr<cv::DescriptorMatcher>matcher = cv::DescriptorMatcher::create("BruteForce"); // マッチング方法：総当たり法・L2ノルム
    //std::vector<DMatch> matches; // マッチング情報
    cv::Mat best_match_img; // ベストのマッチング画像を保存しておくMat
    float best_score = 100000.0; // XX位の中で最も小さい特徴量距離
    int count = 0;
    while (1) {
        // 現在のフレーム取得
        cv::Mat frame_16bit = GetLiveImage();
        cv::Mat frame_16bit_normalized = frame_16bit.clone();
        cv::Mat frame;
        frame_16bit_normalized = frame_16bit - hist_min; // 下限（hist_min）以下の画素値を0にする
        frame_16bit_normalized = frame_16bit_normalized + cv::Scalar(hist_min + (65536 - hist_max)); // 上限（hist_max）以上の画素値をMaxにする
        cv::normalize(frame_16bit_normalized, frame_16bit_normalized, 0, 4096, cv::NORM_MINMAX); // 正規化
        frame_16bit_normalized.convertTo(frame, CV_8U, 1.0 / 16); // 12bit -> 8bit ∴ 1/16

        // 最初だけ個別に計算
        if (count == 0) {
            // prev_* の値の保持
            prev_frame = frame.clone(); // prev_frame
            detector->detectAndCompute(prev_frame, noArray(), prev_keypoints, prev_descriptors); // prev_keypoint, prev_descriptors

            // 画像の保存
            std::ostringstream filename;
            filename << std::setw(6) << std::setfill('0') << count; // 000000, 000001, 000002 ,... というフォーマットになる
            cv::Mat frame_color; // OpenMVSでグレースケール画像を入力するとエラーを吐くためカラー画像に変換
            cv::cvtColor(frame, frame_color, COLOR_GRAY2BGR);
            cv::imwrite(filepath + "H" + filename.str() + ".jpg", frame_color);

            // カメラ移動のための一時停止
            while (1) {
                cv::Mat tmp_frame_16bit = GetLiveImage();
                cv::Mat tmp_frame_16bit_normalized = tmp_frame_16bit.clone();
                cv::Mat tmp_frame;
                tmp_frame_16bit_normalized = tmp_frame_16bit - hist_min; // 下限（hist_min）以下の画素値を0にする
                tmp_frame_16bit_normalized = tmp_frame_16bit_normalized + cv::Scalar(hist_min + (65536 - hist_max)); // 上限（hist_max）以上の画素値をMaxにする
                cv::normalize(tmp_frame_16bit_normalized, tmp_frame_16bit_normalized, 0, 4096, cv::NORM_MINMAX); // 正規化
                tmp_frame_16bit_normalized.convertTo(tmp_frame, CV_8U, 1.0 / 16); // 12bit -> 8bit ∴ 1/16

                cv::putText(tmp_frame, "Move to next position. Then, push 's'.", cv::Point(25, 75), cv::FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(255), 3);
                cv::imshow("view for focus check", tmp_frame); // 大体のピントを目視確認するための確認画面
                int k = cv::waitKey(int(1000 / fps));
                if (k == 115) {
                    cv::destroyWindow("view for focus check");
                    break;
                }
            }

            // 値の更新
            count++;

            continue;
        }

        // キーポイントの抽出と特徴量の計算
        std::vector<cv::KeyPoint> keypoints;
        cv::Mat descriptors;
        detector->detectAndCompute(frame, noArray(), keypoints, descriptors);
        if (keypoints.size() == 0) {
            // キーポイントが一個も検出されなければ撮影し直す
            count--;
            continue; 
        }

        // キーポイントの描画
        cv::Mat frame_with_keypoints;
        cv::drawKeypoints(frame, keypoints, frame_with_keypoints, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

        // 特徴量のマッチング
        std::vector<DMatch> matches;
        matcher->match(prev_descriptors, descriptors, matches);

        // マッチング結果をソートした後、特徴量距離の小さい200点のみ残す。
        for (int i = 0; i < matches.size() - 1 ; i++) {
            double min = matches[i].distance;
            int n = i;
            for (int j = i + 1; j < matches.size(); j++) {
                if (min > matches[j].distance) {
                    n = j;
                    min = matches[j].distance;
                }
            }
            std::swap(matches[i], matches[n]);
        }
        if (matches.size() > 200) {
            matches.erase(matches.begin() + 200, matches.end());
            float tmp_score = matches[199].distance;
        }
        else {
            float tmp_score = matches[matches.size() - 1].distance;
        }

        // マッチング結果の描画(左：一個前のフレーム，右：現在のフレーム)
        cv::Mat match_img;
        cv::drawMatches(prev_frame, prev_keypoints, frame, keypoints, matches, match_img, Scalar(0, 150, 0), Scalar(0, 0, 150), std::vector<char>(), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

        // 200位のスコアがベストより良ければ更新
        if (best_score > matches[matches.size() - 1].distance) {
            best_frame = frame.clone();
            best_keypoints = keypoints;
            best_descriptors = descriptors;
            best_match_img = match_img.clone();
            best_score = matches[matches.size() - 1].distance;
        }

        // 上側にベストの状態、下側に現在の状態を表示する。
        // また、左側にマッチング結果、右側にフレームを出す。
        cv::Mat compare_view, best_frame_color, frame_color, tmpMat1, tmpMat2;
        cvtColor(frame, frame_color, cv::COLOR_GRAY2BGR);  // 画像のチャンネル数を合わせる
        cvtColor(best_frame, best_frame_color, cv::COLOR_GRAY2BGR);  // 画像のチャンネル数を合わせる
        cv::hconcat(best_match_img, best_frame_color, tmpMat1);
        cv::hconcat(match_img, frame_color, tmpMat2);
        cv::vconcat(tmpMat1, tmpMat2, compare_view);
        cv::putText(compare_view, "Best", cv::Point(25, 75), cv::FONT_HERSHEY_SIMPLEX, 1.8, cv::Scalar(255, 255, 255), 4);
        cv::putText(compare_view, "Current", cv::Point(25, 75 + nHeight), cv::FONT_HERSHEY_SIMPLEX, 1.8, cv::Scalar(255, 255, 255), 4);
        cv::putText(compare_view, "Push 's' to save", cv::Point(nWidth + nWidth + 25, 75), cv::FONT_HERSHEY_SIMPLEX, 1.8, cv::Scalar(255, 255, 255), 4);
        cv::putText(compare_view, "Push 'c' to save", cv::Point(nWidth + nWidth + 25, 75 + nHeight), cv::FONT_HERSHEY_SIMPLEX, 1.8, cv::Scalar(255, 255, 255), 4);
        cv::resize(compare_view, compare_view, cv::Size(), 0.5, 0.5);
        cv::imshow("compare view", compare_view);


        /*****************************************************************************************
        * 【キーボード入力に対する処理】
        * Escキーで終了 
        *「s」（キーコード115）が入力されたらベストの画像を保存して次の位置へ 
        *「c」（キーコード99）が入力されたら現在の画像を保存して次の位置へ（ピントによってはこちらを使う方が良い）
        * ****************************************************************************************/
        int k = cv::waitKey(int(1000 / fps));
        if (k == 27) {
            break;

        }else if(k == 115){
            // prev_* の値の保持
            prev_frame = best_frame.clone();
            prev_keypoints = best_keypoints;
            prev_descriptors = best_descriptors;

            // 画像の保存
            std::ostringstream filename;
            filename << std::setw(6) << std::setfill('0') << count; // 000000, 000001, 000002 ,... というフォーマットになる
            cv::Mat best_frame_color; // OpenMVSでグレースケール画像を入力するとエラーを吐くためカラー画像に変換
            cv::cvtColor(best_frame, best_frame_color, COLOR_GRAY2BGR);
            cv::imwrite(filepath + "H" + filename.str() + ".jpg", best_frame_color); // ベストフレームを保存する

        }else if (k == 99) {
            // prev_* の値の保持
            prev_frame = frame.clone();
            prev_keypoints = keypoints;
            prev_descriptors = descriptors;

            // 画像の保存
            std::ostringstream filename;
            filename << std::setw(6) << std::setfill('0') << count; // 000000, 000001, 000002 ,... というフォーマットになる
            cv::Mat frame_color; // OpenMVSでグレースケール画像を入力するとエラーを吐くためカラー画像に変換
            cv::cvtColor(frame, frame_color, COLOR_GRAY2BGR);
            cv::imwrite(filepath + "H" + filename.str() + ".jpg", frame_color);  // 現在のフレームを保存する

        }
        else {
            best_score = 100000.0;
            continue;

        }

        // 以下、「s」か「c」が押された場合の共通処理。

        // 確認画面の削除
        cv::destroyWindow("compare view");

        // カメラ移動のための一時停止
        while (1) {
            cv::Mat tmp_frame_16bit = GetLiveImage();
            cv::Mat tmp_frame_16bit_normalized = tmp_frame_16bit.clone();
            cv::Mat tmp_frame;
            tmp_frame_16bit_normalized = tmp_frame_16bit - hist_min; // 下限（hist_min）以下の画素値を0にする
            tmp_frame_16bit_normalized = tmp_frame_16bit_normalized + cv::Scalar(hist_min + (65536 - hist_max)); // 上限（hist_max）以上の画素値をMaxにする
            cv::normalize(tmp_frame_16bit_normalized, tmp_frame_16bit_normalized, 0, 4096, cv::NORM_MINMAX); // 正規化
            tmp_frame_16bit_normalized.convertTo(tmp_frame, CV_8U, 1.0 / 16); // 12bit -> 8bit ∴ 1/16

            cv::putText(tmp_frame, "Move to next position. Then, push 's'.", cv::Point(25, 75), cv::FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(255), 3);
            cv::imshow("view for focus check", tmp_frame);  // 大体のピントを目視確認するための確認画面
            int k = cv::waitKey(int(1000 / fps));
            if (k == 115) {
                cv::destroyWindow("view for focus check");
                break;
            }
        }

        // 値のリセット・更新
        best_score = 100000.0;
        count++;
    }
}



void Init() {
    unsigned long nRet;
    unsigned long nErrorCode;

    // Initialize(ヘルプ-プログラミング-初期化/終了処理)
    nRet = PDC_Init(&nErrorCode);
    if (nRet == PDC_FAILED) printf("PDC_Init Error %d\n", nErrorCode);
}

PDC_DETECT_NUM_INFO DetectDevice(unsigned long *IPList) {
    unsigned long nRet;
    unsigned long nErrorCode;
    PDC_DETECT_NUM_INFO DetectNumInfo;

    // Search Device(ヘルプ-プログラミング-デバイスの検索)
    nRet = PDC_DetectDevice(PDC_INTTYPE_G_ETHER,    /* Gigabit-Ether I/F */
        IPList,                 /* IPアドレス */
        1,                      /* 最大検索台数 */
        PDC_DETECT_NORMAL,      /* IPアドレスを明示的に指定 */
        &DetectNumInfo,
        &nErrorCode);
    if (nRet == PDC_FAILED) {
        printf("PDC_DetectDevice Error %d\n", nErrorCode);
    }
    // すでにデバイス他のインターフェースで開いている場合
    if (nErrorCode == PDC_ERROR_OPEN_ALREADY) {
        printf("Device was already opened. %lu\n", nErrorCode);
    }
    //検索結果が0台の場合(コード -1)
    if (DetectNumInfo.m_nDeviceNum == 0) {
        exit(-1);
    }
    // 検索結果がFASTCAM - AX Mini50ではない(コード - 2)
    if (DetectNumInfo.m_DetectInfo[0].m_nDeviceCode != PDC_DEVTYPE_FCAM_AX50) {
        exit(-2);
    } 
    // 検索結果のIPアドレスが異なる(コード -3)
    if (DetectNumInfo.m_DetectInfo[0].m_nTmpDeviceNo != IPList[0]) {
        exit(-3);
    }

    return DetectNumInfo;
}

void OpenDevice(PDC_DETECT_NUM_INFO DetectNumInfo) {
    unsigned long nRet;
    unsigned long nErrorCode;

    // OpenDevice
    nRet = PDC_OpenDevice(&(DetectNumInfo.m_DetectInfo[0]), /* 対象デバイス情報 */
        &nDeviceNo,                        /* デバイスナンバー */
        &nErrorCode);
    if (nRet == PDC_FAILED) {
        printf("PDC_OpenDeviceError %d\n", nErrorCode);
    }
}

void SetRecordRate(unsigned long nRate) {
    unsigned long nRet;
    unsigned long nErrorCode;
    unsigned long nCount;                          /* 撮影速度一覧数 */
    unsigned long RateList[PDC_MAX_LIST_NUMBER];   /* 撮影速度一覧 */
    unsigned long nFlag;


    /* そのデバイスにて設定可能な撮影速度一覧を取得 */
    nRet = PDC_GetRecordRateList(nDeviceNo,
        nChildNo,
        &nCount,
        RateList,
        &nErrorCode);

    if (nRet == PDC_FAILED) {
        printf("PDC_GetRecordRateList Error %d\n", nErrorCode);
        return;
    }

    nFlag = FALSE;
    for (int i = 0 ; i < nCount ; i++) {
        if (RateList[i] == nRate) {
            nFlag = TRUE;
            break;
        }
    }

    /* 取得リストにnRate[fps]がない */
    if (!nFlag) {
        return;
    }

    nRet = PDC_SetRecordRate(nDeviceNo,
        nChildNo,
        nRate,
        &nErrorCode);

    if (nRet == PDC_FAILED) {
        printf("PDC_SetRecordRate Error %d\n", nErrorCode);
    }

}

void SetShutterSpeed(unsigned long nFps) {
    unsigned long nRet;
    unsigned long nErrorCode;

    /* そのデバイスにて設定可能な撮影速度一覧を取得 */
    nRet = PDC_GetShutterSpeedFps(nDeviceNo,
        nChildNo,
        &nFps,
        &nErrorCode);

    if (nRet == PDC_FAILED) {
        printf("PDC_GetShutterSpeedFps Error %d\n", nErrorCode);
        return;
    }

    nRet = PDC_SetShutterSpeedFps(nDeviceNo,
        nChildNo,
        nFps,
        &nErrorCode);

    if (nRet == PDC_FAILED) {
        printf("PDC_SetShutterSpeed Error %d\n", nErrorCode);
    }

}

void Shading() {
    unsigned long nRet;
    unsigned long nErrorCode;
    unsigned long nStatus; /* デバイス状態 */

    /* キャリブレーションを行い、シェーディング補正を有効にする */
    /* このとき、イメージセンサは完全に遮光した状態にしておいて下さい */
    nRet = PDC_SetShadingMode(nDeviceNo,
        nChildNo,
        PDC_SHADING_ON,
        &nErrorCode);

    if (nRet == PDC_FAILED) {
        printf("PDC_SetShadingMode Error %d\n", nErrorCode);
    }

    /* ヘルプにはシェーディングデータの保存方法まで説明されているが、
    AX Miniは対応していないのかエラーが出るため、シェーディングの実行に留めている。*/
}

void SetStatus() {
    /* 現在の動作モードを取得 */
    unsigned long nRet;
    unsigned long nErrorCode;
    unsigned long nStatus;     /* デバイス状態 */
    nRet = PDC_GetStatus(nDeviceNo,
        &nStatus,
        &nErrorCode);

    if (nRet == PDC_FAILED) {
        printf("PDC_GetStatus Error %d\n", nErrorCode);
    }

    /* メモリ再生モードの場合はライブモードに切り替える */
    if (nStatus == PDC_STATUS_PLAYBACK) {
        nRet = PDC_SetStatus(nDeviceNo,
            PDC_STATUS_LIVE,
            &nErrorCode);

        if (nRet == PDC_FAILED) {
            printf("PDC_SetStatus Error %d\n", nErrorCode);
        }
    }
}

cv::Mat GetLiveImage() {
    /* ライブ画像の取得・表示 */
    unsigned long nRet;
    unsigned long nErrorCode;
    unsigned short* pBuf; /* ライブ画像を格納するメモリ配列のポインタ */
    pBuf = (unsigned short*)malloc(sizeof(unsigned short) * nWidth * nHeight);

    nRet = PDC_GetLiveImageData(nDeviceNo,
		nChildNo,
		16,  /* 16ビット */
		pBuf,
		&nErrorCode);

    if (nRet == PDC_FAILED) {
        printf("PDC_GetLiveImageData Error %d\n", nErrorCode);
        free(pBuf);
        exit(-4);
    }

    // pBuf : 画像左上を原点とするバイナリファイル
    cv::Mat frame = cv::Mat::zeros(nWidth, nHeight, CV_16U);
    int index;
    for (int y = 0; y < nHeight; y++) {
        for (int x = 0; x < nWidth; x++) {
            index = y * nWidth + x;
            frame.at<unsigned short>(y, x) = pBuf[index];
        }
    }

    //// ヒストグラム正規化
    //cv::equalizeHist(frame, frame);

    free(pBuf);

    return frame;
}

void showHist(cv::Mat frame_16bit) {
    cv::Mat hist;
    int N = 4096;
    Mat hist_img(Size(N, N), CV_8UC3, Scalar::all(255));
    Mat targetImg(Size(N, N), CV_8UC3, Scalar::all(255));
    const int hdims[] = { N }; // 次元毎のヒストグラムサイズ
    const float hranges[] = { 0, N };
    const float* ranges[] = { hranges }; // 次元毎のビンの下限上限
    double max_val = .0;

    // ヒストグラムの計算
    cv::calcHist(&frame_16bit, 1, 0, Mat(), hist, 1, hdims, ranges);

    // 最大値の計算					
    minMaxLoc(hist, 0, &max_val);

    // ヒストグラムのスケーリング					
    hist = hist * (max_val ? N / max_val : 0.);

    // ヒストグラムの画像作成
    int bin_w = saturate_cast<int>((double)N / hdims[0]);
    for (int j = 0; j < hdims[0]; ++j) {
        rectangle(hist_img, Point(j * bin_w, hist_img.rows), Point((j + 1) * bin_w, hist_img.rows - saturate_cast<int>(hist.at<float>(j))), Scalar::all(128), -1);
    }
    rectangle(hist_img, Point((hist_min - 2) * bin_w, 0), Point((hist_min + 2) * bin_w, hist_img.rows), cv::Scalar(0, 0, 200), -1);
    rectangle(hist_img, Point((hist_max - 2) * bin_w, 0), Point((hist_max + 2) * bin_w, hist_img.rows), cv::Scalar(0, 0, 200), -1);

    // トラックバーを表示するウィンドウを作成
    cv::namedWindow("histgram");

    // ヒストグラム画像表示
    resize(hist_img, hist_img, cv::Size(), 0.2, 0.2);
    imshow("histgram", hist_img);

    // トラックバー作成
    cv::createTrackbar("max", "histgram", &hist_max, N);
    cv::createTrackbar("min", "histgram", &hist_min, N);
}

String MakeDirWithTime(String dir_path) {
    // 現在時刻の文字列を生成（例：20200101120556）
    std::stringstream time_string;
    time_t t = time(NULL);
    struct tm localTime;
    errno_t error = localtime_s(&localTime, &t);
    time_string << "20" << localTime.tm_year - 100;
    time_string << std::setw(2) << std::setfill('0') << localTime.tm_mon + 1;
    time_string << std::setw(2) << std::setfill('0') << localTime.tm_mday;
    time_string << std::setw(2) << std::setfill('0') << localTime.tm_hour;
    time_string << std::setw(2) << std::setfill('0') << localTime.tm_min;
    time_string << std::setw(2) << std::setfill('0') << localTime.tm_sec;

    // ディレクトリ作成
    _mkdir((dir_path + time_string.str() + "/").c_str());

    // ディレクトリ名をreturn
    return dir_path + time_string.str() + "/";
}


// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
