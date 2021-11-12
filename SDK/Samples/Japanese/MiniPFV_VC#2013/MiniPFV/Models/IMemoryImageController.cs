using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;
using System.Windows;



namespace MiniPFV.Models
{
    public interface IMemoryImageController
    {
        /// <summary>
        /// メモリ画像情報の取得
        /// </summary>
        void GetMemImageInfo();

        /// <summary>
        /// メモリ画像フレーム情報
        /// </summary>
        PDC_FRAME_INFO FrameInfo { get; }

        /// <summary>
        /// メモリ画像の取得
        /// </summary>
        void GetMemImageData(int frameNo);

        /// <summary>
        /// メモリ画像
        /// </summary>
        BitmapSource MemImageSource {get;}

        /// <summary>
        /// 画像ファイル保存
        /// </summary>
        void SaveFile(string fileName, int startFrameNo, int endFrameNo, Window savingFileWindow);

        /// <summary>
        /// ファイル保存をキャンセル
        /// </summary>
        void CancelSaveFile();

        /// <summary>
        /// メモリ画像の撮影速度を取得
        /// </summary>
        UInt32 RecordRate { get; }

        /// <summary>
        /// メモリ画像のシャッタースピードを取得
        /// </summary>
        UInt32 RecordShutterSpeed { get; }

        /// <summary>
        /// メモリ画像の解像度を取得
        /// </summary>
        Resolution RecordResolution { get; }

    }
}
