using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;


namespace MiniPFV.Models
{
    public interface ILiveImageController
    {
        /// <summary>
        /// 撮影速度リストの取得
        /// </summary>
        IList<UInt32> GetRecordRateList();

        /// <summary>
        /// 撮影速度の設定
        /// </summary>
        void SetRecordRate(UInt32 selectedRecordRate);

        /// <summary>
        /// シャッタースピードリストの取得
        /// </summary>
        IList<UInt32> GetShutterSpeedList();


        /// <summary>
        /// シャッタースピードの設定
        /// </summary>
        void SetShutterSpeed(UInt32 selectedShutterSpeed);

        /// <summary>
        /// 解像度リストの取得
        /// </summary>
        IList<Resolution> GetResolutionList();

        /// <summary>
        /// 解像度の設定
        /// </summary>
        void SetResolution(Resolution selectedResolution);

        /// <summary>
        /// ライブ画像データ
        /// </summary>
        BitmapSource LiveImageSource { get; }

        /// <summary>
        /// ライブ画像データの取得
        /// </summary>
        void GetLiveImageData(Resolution selectedResolution, ColorType colorType);

        /// <summary>
        /// 拡大率リストの取得
        /// </summary>
        List<Magnification> MagnificationList { get; }

        /// <summary>
        /// 録画開始
        /// </summary>
        void RecordStart();

        /// <summary>
        /// 録画終了
        /// </summary>
        void RecordStop();

        /// <summary>
        /// 録画状態の取得
        /// </summary>
        /// <returns>録画状態:"LIVE", "RECORD READY", "RECORDING"</returns>
        string GetStatus();

    }
}
