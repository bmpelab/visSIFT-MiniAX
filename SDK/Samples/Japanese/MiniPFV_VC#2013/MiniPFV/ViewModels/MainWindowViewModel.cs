using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

using Livet;
using Livet.Commands;
using Livet.Messaging;
using Livet.Messaging.IO;
using Livet.EventListeners;
using Livet.Messaging.Windows;

using MiniPFV.Models;
using System.Threading.Tasks;
using System.Windows.Threading;

using System.Runtime.InteropServices;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.IO;

using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Forms;

namespace MiniPFV.ViewModels
{
    public class MainWindowViewModel : ViewModel
    {
        /* コマンド、プロパティの定義にはそれぞれ 
         * 
         *  lvcom   : ViewModelCommand
         *  lvcomn  : ViewModelCommand(CanExecute無)
         *  llcom   : ListenerCommand(パラメータ有のコマンド)
         *  llcomn  : ListenerCommand(パラメータ有のコマンド・CanExecute無)
         *  lprop   : 変更通知プロパティ(.NET4.5ではlpropn)
         *  
         * を使用してください。
         * 
         * Modelが十分にリッチであるならコマンドにこだわる必要はありません。
         * View側のコードビハインドを使用しないMVVMパターンの実装を行う場合でも、ViewModelにメソッドを定義し、
         * LivetCallMethodActionなどから直接メソッドを呼び出してください。
         * 
         * ViewModelのコマンドを呼び出せるLivetのすべてのビヘイビア・トリガー・アクションは
         * 同様に直接ViewModelのメソッドを呼び出し可能です。
         */

        /* ViewModelからViewを操作したい場合は、View側のコードビハインド無で処理を行いたい場合は
         * Messengerプロパティからメッセージ(各種InteractionMessage)を発信する事を検討してください。
         */

        /* Modelからの変更通知などの各種イベントを受け取る場合は、PropertyChangedEventListenerや
         * CollectionChangedEventListenerを使うと便利です。各種ListenerはViewModelに定義されている
         * CompositeDisposableプロパティ(LivetCompositeDisposable型)に格納しておく事でイベント解放を容易に行えます。
         * 
         * ReactiveExtensionsなどを併用する場合は、ReactiveExtensionsのCompositeDisposableを
         * ViewModelのCompositeDisposableプロパティに格納しておくのを推奨します。
         * 
         * LivetのWindowテンプレートではViewのウィンドウが閉じる際にDataContextDisposeActionが動作するようになっており、
         * ViewModelのDisposeが呼ばれCompositeDisposableプロパティに格納されたすべてのIDisposable型のインスタンスが解放されます。
         * 
         * ViewModelを使いまわしたい時などは、ViewからDataContextDisposeActionを取り除くか、発動のタイミングをずらす事で対応可能です。
         */

        /* UIDispatcherを操作する場合は、DispatcherHelperのメソッドを操作してください。
         * UIDispatcher自体はApp.xaml.csでインスタンスを確保してあります。
         * 
         * LivetのViewModelではプロパティ変更通知(RaisePropertyChanged)やDispatcherCollectionを使ったコレクション変更通知は
         * 自動的にUIDispatcher上での通知に変換されます。変更通知に際してUIDispatcherを操作する必要はありません。
         */

        public const UInt32 LIVE_MODE = 0;
        public const UInt32 MEMORY_MODE = 1;


        public void Initialize()
        {
        }
        
        private readonly IControlCamera _control;
        ILiveImageController _controlLiveImage;
        IMemoryImageController _controlMemImage;

        public MainWindowViewModel(IControlCamera control)
        {
            _control = control;
            if (_control.CameraHeads.Count > 0)
            {
                // ヘッドリストの最初のカメラヘッドを選択する
                selectedCameraHead = _control.CameraHeads[0];
                
                // ライブ画像のコントロールとメモリ画像のコントロールを作成
                _controlLiveImage = new LiveImageController(selectedCameraHead.DeviceNo, selectedCameraHead.ChildNo, selectedCameraHead.ColorType);
                _controlMemImage = new MemoryImageController(selectedCameraHead.DeviceNo, selectedCameraHead.ChildNo, selectedCameraHead.ColorType, selectedCameraHead.DeviceName);
                
                // 初期拡大率を100％に設定
                SelectedMagnification = _controlLiveImage.MagnificationList[3];

                // カメラ初期撮影速度設定
                rateList = _controlLiveImage.GetRecordRateList();
                SelectedFrameRate = rateList[0];

                // 別スレッドでライブ画像を取得
                Task.Factory.StartNew(RealtimeLoadImage);
            }
            else
            {
                errorMessage = "No Camera Head.";
            }
        }



        public string CameraName
        {
            get {            

                return selectedCameraHead.DeviceName;

            }
        }

        string errorMessage;
        public string ErrorMessage
        {
            get { return errorMessage; }
        }


        public IList<IControlCameraHead> CameraHeads
        {
            get { return _control.CameraHeads; }
        }

        IControlCameraHead selectedCameraHead;

        public IControlCameraHead SelectedCameraHead
        {
            get { return selectedCameraHead; }
            set 
            {
                selectedCameraHead = value;
            }
        }

        IList<UInt32> rateList;
        public IList<UInt32> RateList
        {
            get {
                try
                {
                    // 撮影速度リストを取得
                    rateList = _controlLiveImage.GetRecordRateList();
                }
                catch (PdclibException ex)
                {
                    Messenger.Raise(new InformationMessage("Error: " + ex.Message + "\n" + "Get Frame rate list failed.", "", MessageBoxImage.Error, "Information"));                  
                }
                return rateList; }
          
        }

        UInt32 selectedFrameRate;
        public UInt32 SelectedFrameRate
        {
            get { return selectedFrameRate; }
            set
            {
                selectedFrameRate = value;
                try
                {
                    // 選択した撮影速度に設定する
                    _controlLiveImage.SetRecordRate(selectedFrameRate);

                    // 撮影速度を変更すると、シャッタースピードと解像度も対応するリストに更新する
                    RaisePropertyChanged(() => ShutterSpeedList);
                    RaisePropertyChanged(() => ResolutionList);

                    // リスト最初の値に設定する
                    SelectedShutterSpeed = ShutterSpeedList[0];
                    RaisePropertyChanged(() => SelectedShutterSpeed);
                    SelectedResolution = ResolutionList[0];
                    RaisePropertyChanged(() => SelectedResolution);
                }
                catch (PdclibException ex)
                {
                    Messenger.Raise(new InformationMessage("Error: " + ex.Message + "\n" + "Frame rate set failed." , "", MessageBoxImage.Error, "Information"));

                }

            }        
        }

        IList<UInt32> shutterSpeedList;
        public IList<UInt32> ShutterSpeedList
        {
            get {
                try
                {
                    // シャッタースピードリストを取得
                    shutterSpeedList = _controlLiveImage.GetShutterSpeedList();
                }
                catch (PdclibException ex)
                {
                    Messenger.Raise(new InformationMessage("Error: " + ex.Message + "\n" + "Get shutter speed list failed.", "", MessageBoxImage.Error, "Information"));
                }
                return shutterSpeedList;
            }

       }

        UInt32 selectedShutterSpeed;
        public UInt32 SelectedShutterSpeed
        {
            get { return selectedShutterSpeed; }
            set
            {
                selectedShutterSpeed = value;
                try
                {
                    // 選択したシャッタースピードに設定する
                    _controlLiveImage.SetShutterSpeed(selectedShutterSpeed);
                }
                catch (PdclibException ex)
                {
                    Messenger.Raise(new InformationMessage("Error: " + ex.Message + "\n" + "Set shutter speed failed.", "", MessageBoxImage.Error, "Information"));
                }
            }
        }

        IList<Resolution> resolutionList;
        public IList<Resolution> ResolutionList
        {
            get
            {
                try
                {
                    // 解像度リストを取得
                    resolutionList = _controlLiveImage.GetResolutionList();
                }
                catch (PdclibException ex)
                {
                        Messenger.Raise(new InformationMessage("Error: " + ex.Message + "\n" + "Get resolution list failed.", "", MessageBoxImage.Error, "Information"));

                }
                return resolutionList;
            }
        }

        Resolution selectedResolution;
        public Resolution SelectedResolution
        {
            get
            {
                return selectedResolution;
            }
            set
            {
               selectedResolution = value;
               try
                {
                    // 選択した解像度に設定する
                    _controlLiveImage.SetResolution(selectedResolution);
                }
                catch (PdclibException ex)
                {
                    Messenger.Raise(new InformationMessage("Error: " + ex.Message + "\n" + "Set resolution failed.", "", MessageBoxImage.Error, "Information"));
                }

            }
        }

        public BitmapSource LiveImageSource
        {
            get { return _controlLiveImage.LiveImageSource; }
        }


        public bool IsWindowClosed = false;
        public void RealtimeLoadImage()
        {
            // ライブモードを選択する、ウインドを閉じないときライブ画像を取得する
            while (SelectedTab == LIVE_MODE && IsWindowClosed == false)
            {
                //　UIスレッドで実行
                DispatcherHelper.UIDispatcher.Invoke(() =>
                    {
                        try
                        {
                            _controlLiveImage.GetLiveImageData(SelectedResolution, selectedCameraHead.ColorType);
                            RaisePropertyChanged(() => LiveImageSource);
                        }
                        catch (PdclibException ex)
                        {
                            //　ライブモードでウインドを閉じないときだけエラーを表示する
                            if (SelectedTab == LIVE_MODE && IsWindowClosed == false)
                            {
                                Messenger.Raise(new InformationMessage("Error: " + ex.Message + "\n" + "Get live image failed.", "", MessageBoxImage.Error, "Information"));

                            }
                        }
                    });
                Thread.Sleep(1);
            }

        }


        UInt32 selectedTab = LIVE_MODE;
        UInt32 cameraMode;
        public UInt32 SelectedTab
        {
            get { return selectedTab; }
            set
            {
                selectedTab = value;
                // 選択したタブによってカメラモードを切り替える
                cameraMode = selectedTab;
                try
                {
                    SetCameraStatus(cameraMode);
                }
                catch (PdclibException ex)
                {
                    Messenger.Raise(new InformationMessage("Error: " + ex.Message + "\n" + "Convert camera mode failed.", "", MessageBoxImage.Error, "Information"));
                }
            }
        }


        // モードを切り替えて初期状態を設定する
        void SetCameraStatus(UInt32 cameraMode)
        {
            CameraModeManager modeManager = new CameraModeManager(selectedCameraHead.DeviceNo, cameraMode);
            modeManager.SetStatus();

            // ライブモード
            if (cameraMode == LIVE_MODE)
            {
                // ライブ画像を表示する
                Task.Factory.StartNew(RealtimeLoadImage);
            }
            // メモリモード
            else
            {
                // メモリ画像の情報を取得して、表示する
                _controlMemImage.GetMemImageInfo();
                RaisePropertyChanged(() => TotalFrameNo);
                RaisePropertyChanged(() => RecordRate);
                RaisePropertyChanged(() => RecordShutterSpeed);
                RaisePropertyChanged(() => RecordResolution);

                // 初期状態でメモリに最初の画像を表示する
                _controlMemImage.GetMemImageData(_controlMemImage.FrameInfo.m_nStart);
                RaisePropertyChanged(() => MemImageSource);

                startFrameNo = (UInt32)_controlMemImage.FrameInfo.m_nStart;
                RaisePropertyChanged(() => StartFrameNo);

                currentFrameNo = (UInt32)_controlMemImage.FrameInfo.m_nStart;
                RaisePropertyChanged(() => CurrentFrameNo);

                endFrameNo = (UInt32)_controlMemImage.FrameInfo.m_nEnd;
                RaisePropertyChanged(() => EndFrameNo);

                // 初期状態で画像を保存する範囲は全メモリ画像に設定する
                saveFileStartFrameNo = _controlMemImage.FrameInfo.m_nStart;
                RaisePropertyChanged(() => SaveFileStartFrameNo);

                saveFileEndFrameNo = _controlMemImage.FrameInfo.m_nEnd;
                RaisePropertyChanged(() => SaveFileEndFrameNo);

            }

        }

        public UInt32 RecordRate
        {
            get { return _controlMemImage.RecordRate; }
        }

        public UInt32 RecordShutterSpeed
        {
            get { return _controlMemImage.RecordShutterSpeed; }
        }

        public Resolution RecordResolution
        {
            get { return _controlMemImage.RecordResolution; }
        }


        public List<Magnification> MagnificationList
        {
            get { return _controlLiveImage.MagnificationList; }
        }

        Magnification selectedMagnification;
        public Magnification SelectedMagnification
        {
            get { return selectedMagnification; }
            set
            {
                selectedMagnification = value;
                RaisePropertyChanged(() => SelectedMagnification);

            }
        }


        bool isRecordStopAvailable = false;
        bool isRecordStartAvailable = true;
        public void RecordStart()
        {
            try
            {
                // 録画開始すると、Record Stopボタンを有効、Record Startボタンを無効にする
                isRecordStopAvailable = true;
                RaisePropertyChanged(() => IsRecordStopAvailable);
                isRecordStartAvailable = false;
                RaisePropertyChanged(() => IsRecordStartAvailable);

                _controlLiveImage.RecordStart();
            }
            catch (PdclibException ex)
            {
                Messenger.Raise(new InformationMessage("Error: " + ex.Message + "\n" + "Can not start recording!", "", MessageBoxImage.Error, "Information"));
            }
            // 別スレッドで録画状態をチェックする
            Task.Factory.StartNew(GetStatus);
 
        }

        public void GetStatus()
        {
            try
            {
                while (true)
                {
                    // 録画状態を取得
                    cameraStatus = _controlLiveImage.GetStatus();
                    // UIスレッドに録画状態を更新する
                    DispatcherHelper.UIDispatcher.Invoke(() =>
                    {
                        RaisePropertyChanged(() => CameraStatus);
                    });

                    if (cameraStatus == "LIVE")
                        break;

                    Thread.Sleep(1);
                }

                // 録画終わった後メモリモードに切り替える(自動停止する場合）
                DispatcherHelper.UIDispatcher.Invoke(() =>
                    {
                        selectedTab = MEMORY_MODE;
                        RaisePropertyChanged(() => SelectedTab);
                        cameraMode = selectedTab;
                        SetCameraStatus(cameraMode);

                        // 録画ボタンを初期状態に戻る
                        isRecordStopAvailable = false;
                        RaisePropertyChanged(() => IsRecordStopAvailable);
                        isRecordStartAvailable = true;
                        RaisePropertyChanged(() => IsRecordStartAvailable);

                    });

            }
            catch (PdclibException ex)
            {
                Messenger.Raise(new InformationMessage("Error: " + ex.Message + "\n" + "Can not get camera status.", "", MessageBoxImage.Error, "Information"));
            }

        }

        public void RecordStop()
        {
            try
            {
                isRecordStopAvailable = false;
                RaisePropertyChanged(() => IsRecordStopAvailable);
                isRecordStartAvailable = true;
                RaisePropertyChanged(() => IsRecordStartAvailable);

                _controlLiveImage.RecordStop();

                // 録画終わったらメモリモードに切り替える
                selectedTab = MEMORY_MODE;
                RaisePropertyChanged(() => SelectedTab);
                cameraMode = selectedTab;
                SetCameraStatus(cameraMode);
                
            }
            catch (PdclibException ex)
            {
                Messenger.Raise(new InformationMessage("Error: " + ex.Message + "\n" + "Can not stop recording.", "", MessageBoxImage.Error, "Information"));
            }

        }

        public bool IsRecordStopAvailable
        {
            get { return isRecordStopAvailable; }
            set { isRecordStopAvailable = value;}
        }

        public bool IsRecordStartAvailable
        {
            get { return isRecordStartAvailable; }
            set { isRecordStartAvailable = value; }
        }

        string cameraStatus = "LIVE";
        public string CameraStatus
        {
            get {
                // 録画中撮影速度、シャッタースピード、解像度の設定を無効にする
                if (cameraStatus != "LIVE")
                    isSettingEnable = false;
                else
                    isSettingEnable = true;
                RaisePropertyChanged(() => IsSettingEnable);
                return cameraStatus; }
        }

        bool isSettingEnable = true;
        public bool IsSettingEnable
        {
            get { return isSettingEnable; }
        }

        public void GetMemImage()
        {            
            // 別スレッドで実行
            Task.Factory.StartNew(GetMemImageData);
        }

        UInt32 startFrameNo;
        UInt32 currentFrameNo;
        UInt32 endFrameNo;

        public void GetMemImageData()
        {
            isStop = false;
            // 現在フレームは再生範囲以外にあるときは開始フレームから再生する
            if (currentFrameNo < startFrameNo || currentFrameNo > endFrameNo)
            {
                currentFrameNo = startFrameNo;
            }

            // 現在フレームから再生する
            for (int i = (int)currentFrameNo; i <= endFrameNo; i++)
            {
                // UIスレッドでメモリ画像を更新する
                DispatcherHelper.UIDispatcher.Invoke(() =>
                       {
                           try
                           {
                               _controlMemImage.GetMemImageData(i);
                               RaisePropertyChanged(() => MemImageSource);
                               RaisePropertyChanged(() => CurrentFrameNo);
                           }
                           catch (PdclibException ex)
                           {
                               Messenger.Raise(new InformationMessage("Error: " + ex.Message + "\n" + "Can not get memory images.", "", MessageBoxImage.Error, "Information"));
                               isStop = true;
                           }
                       });
                currentFrameNo = (UInt32)i+1;
                Thread.Sleep(10);
                // 停止もしくは一時停止されたら抜ける
                if (isStop)
                    break;
            }
            isStop = true;

        }

        bool isStop = false;
        // 再生停止
        public void StopPlayingMemImages()
        {
            if (!isStop)
            {
                isStop = true;
            }    

            // 再生開始フレームに戻る
            try
            {
                _controlMemImage.GetMemImageData((int)startFrameNo);
                currentFrameNo = startFrameNo;
                RaisePropertyChanged(() => MemImageSource);
                RaisePropertyChanged(() => CurrentFrameNo);
            }
            catch (PdclibException ex)
            {
                Messenger.Raise(new InformationMessage("Error: " + ex.Message + "\n" + "Can not stop playing.", "", MessageBoxImage.Error, "Information"));
            }
            
        }

        // 再生一時停止
        public void PausePlayingMemImages()
        {
            if (!isStop)
            {
                isStop = true;
            }
        }

        // 現在フレーム
        public UInt32 CurrentFrameNo
        {
            get { return currentFrameNo; }
            set {
                try
                {
                    if (isStop)
                    {
                        // 停止中スライダーを動かすと現在フレームが変わる
                        currentFrameNo = value;
                        _controlMemImage.GetMemImageData((int)currentFrameNo);
                        RaisePropertyChanged(() => MemImageSource);
                    }
                    else
                    {
                        // 再生中にスライダーを動かすと現在フレームが変わる
                        isStop = true;
                        currentFrameNo = value;
                        _controlMemImage.GetMemImageData((int)currentFrameNo);
                        RaisePropertyChanged(() => MemImageSource);
                        // 現在フレームから再生する
                        GetMemImage();
                    }
                }
                catch (PdclibException ex)
                {
                    Messenger.Raise(new InformationMessage("Error: " + ex.Message, "", MessageBoxImage.Error, "Information"));
                }

            }
        }

        // 再生開始フレーム
        public UInt32 StartFrameNo
        {
            get { return startFrameNo; }
            set {
                if (value < _controlMemImage.FrameInfo.m_nStart)
                    startFrameNo = (UInt32)_controlMemImage.FrameInfo.m_nStart;
                else
                    startFrameNo = value;
                // 開始フレームを現在フレームより大きい数字に設定するとき、現在フレームも開始フレームに更新
                if (currentFrameNo < startFrameNo) 
                {
                    currentFrameNo = startFrameNo; 
                }

                // 保存開始フレームを同期する
                saveFileStartFrameNo = (int)startFrameNo;
                
                RaisePropertyChanged(() => StartFrameNo);
                RaisePropertyChanged(() => SaveFileStartFrameNo);
                RaisePropertyChanged(() => CurrentFrameNo);
            }
        }

        public UInt32 EndFrameNo
        {
            get { return endFrameNo; }
            set {
                if (value > _controlMemImage.FrameInfo.m_nEnd)
                    endFrameNo = (UInt32)_controlMemImage.FrameInfo.m_nEnd;
                else
                    endFrameNo = value;
                RaisePropertyChanged(() => EndFrameNo);

                // 保存終了フレームを同期する
                saveFileEndFrameNo = (int)endFrameNo;
                RaisePropertyChanged(() => SaveFileEndFrameNo);
               
            }
        }

        public UInt32 TotalFrameNo
        {
            get { return _controlMemImage.FrameInfo.m_nRecordedFrames; }
        }

        public BitmapSource MemImageSource
        {
            get { return _controlMemImage.MemImageSource; }
        }

        // 画像ファイルを保存する
        public void SaveImages(SavingFileSelectionMessage fileName)
        {
            if (fileName.Response != null)
            { 
                // ファイル保存状態ウインドを作成
                Window savingFileWindow = new MiniPFV.Views.SavingFileWindow { DataContext = new MiniPFV.ViewModels.SavingFileWindowViewModel(_controlMemImage) };
                _controlMemImage.SaveFile(fileName.Response[0], saveFileStartFrameNo, saveFileEndFrameNo, savingFileWindow);
                savingFileWindow.Show();
            }
                       
        }



        int saveFileStartFrameNo;
        int saveFileEndFrameNo;

        // 画像保存開始フレーム
        public int SaveFileStartFrameNo
        {
            get { return saveFileStartFrameNo; }
            set {
                if (value < _controlMemImage.FrameInfo.m_nStart)
                    saveFileStartFrameNo = _controlMemImage.FrameInfo.m_nStart;
                else
                    saveFileStartFrameNo = value;
            }
        }

        // 画像保存終了フレーム
        public int SaveFileEndFrameNo
        {
            get { return saveFileEndFrameNo; }
            set {
                if (value > _controlMemImage.FrameInfo.m_nEnd)
                    saveFileEndFrameNo = _controlMemImage.FrameInfo.m_nEnd;
                else
                    saveFileEndFrameNo = value;
            }
        }      
    }
}
