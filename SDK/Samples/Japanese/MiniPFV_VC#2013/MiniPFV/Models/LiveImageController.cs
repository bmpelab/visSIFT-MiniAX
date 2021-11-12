using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Livet;
using System.Runtime.InteropServices;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.IO;
using System.Windows;
using System.Windows.Threading;
using System.Threading;

namespace MiniPFV.Models
{
    // カメラの状態
    public enum CameraStatus
    {
        PDC_STATUS_LIVE = 0,
        PDC_STATUS_RECREADY = 2,
        PDC_STATUS_REC = 8,
    };

    // ライブモードの画像コントロール
    public class LiveImageController : NotificationObject, ILiveImageController
    {
        // カメラ変数リストに用意するメモリ配列のサイズ
        protected const UInt32 PDC_MAX_LIST_NUMBER = 256;

        // ビットマップ画像のPixelFormat
        protected PixelFormat pfColor = PixelFormats.Bgr24;
        protected PixelFormat pfMono = PixelFormats.Gray8;
        
        // ビットマップ画像のdpi
        protected const double dpiX = 96;
        protected const double dpiY = 96;
        protected const UInt32 PDC_ERROR_LIVEONLY = 110;

        UInt32 _deviceNo;
        UInt32 _childNo;
        ColorType _colorType;

        public LiveImageController(UInt32 deviceNo, UInt32 childNo, ColorType colorType)
        {
            _colorType = colorType;
            _deviceNo = deviceNo;
            _childNo = childNo;
        }

        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_GetRecordRateList(UInt32 deviceNo, UInt32 childDeviceNo, out UInt32 count, UInt32[] frameRate, out UInt32 errorCode);

        // 撮影速度リストの取得
        public IList<UInt32> GetRecordRateList()
        {
            IList<UInt32> rateList = new List<UInt32>();
            UInt32[] frameRate = new UInt32[PDC_MAX_LIST_NUMBER];
            UInt32 count;
            UInt32 errorCode;

            // 指定デバイスの設定可能な撮影速度リストを取得する
            UInt32 ret = PDC_GetRecordRateList(_deviceNo, _childNo, out count, frameRate, out errorCode);
            if (ret == 0)
            {
                var ex = new PdclibException("PDC_GetRecordRateList " + errorCode.ToString());
                throw ex;
            }

            // count分生成
            rateList = new List<UInt32>();
            for (int i = 0; i < count; i++)
            {
                rateList.Add(frameRate[i]);
            }

            return rateList;
        }

        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_SetRecordRate(UInt32 deviceNo, UInt32 childDeviceNo, UInt32 rate, out UInt32 errorCode);

        int n = 0;
        // 撮影速度の設定
        public void SetRecordRate(UInt32 selectedFrameRate)
        {
            UInt32 errorCode;

            // 指定デバイスの撮影速度を設定する
            UInt32 ret = PDC_SetRecordRate(_deviceNo, _childNo, selectedFrameRate, out errorCode);
            if (ret == 0)
            {
                var ex = new PdclibException("PDC_SetRecordRate " + errorCode.ToString());
                throw ex;
            }

        }


        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_GetShutterSpeedFpsList(UInt32 deviceNo, UInt32 childDeviceNo, out UInt32 count, UInt32[] shutterSpeed, out UInt32 errorCode);

        // シャッタースピードリストの取得
        public IList<UInt32> GetShutterSpeedList()
        {
            IList<UInt32> shutterSpeedList;
            UInt32[] shutterSpeed = new UInt32[PDC_MAX_LIST_NUMBER];
            UInt32 count;
            UInt32 errorCode;

            // 指定デバイスで現在設定可能なシャッタースピードリストを取得する
            UInt32 ret = PDC_GetShutterSpeedFpsList(_deviceNo, _childNo, out count, shutterSpeed, out errorCode);
            if (ret == 0)
            {
                var ex = new PdclibException("PDC_GetShutterSpeedFpsList " + errorCode.ToString());
                throw ex;
            }

            // count分生成
            shutterSpeedList = new List<UInt32>();
            for (int i = 0; i < count; i++)
            {
                shutterSpeedList.Add(shutterSpeed[i]);
            }

            return shutterSpeedList;
        }


        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_SetShutterSpeedFps(UInt32 dviceNo, UInt32 childDeviceNo, UInt32 shutterSpeed, out UInt32 errorCode);

        // シャッタースピード設定
        public void SetShutterSpeed(UInt32 selectedShutterSpeed)
        {
            UInt32 errorCode;

            // 指定デバイスのシャッタースピードを設定する
            UInt32 ret = PDC_SetShutterSpeedFps(_deviceNo, _childNo, selectedShutterSpeed, out errorCode);
            if (ret == 0)
            {
                var ex = new PdclibException("PDC_SetShutterSpeedFps " + errorCode.ToString());
                throw ex;
            }
        }
        
        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_GetResolutionList(UInt32 deviceNo, UInt32 childDeviceNo, out UInt32 count, UInt32[] resolution, out UInt32 errorCode);

        // 解像度リストを取得
        public IList<Resolution> GetResolutionList()
        {
            IList<Resolution> resolutionList;
            UInt32 count;
            UInt32[] resolution = new UInt32[PDC_MAX_LIST_NUMBER];
            UInt32 errorCode;

            // 指定デバイスで現在設定可能な解像度リストを取得する
            UInt32 ret = PDC_GetResolutionList(_deviceNo, _childNo, out count, resolution, out errorCode);
            if (ret == 0)
            {
                var ex = new PdclibException("PDC_GetResolutionList " + errorCode.ToString());
                throw ex;
            }
            
            // count分生成
            resolutionList = new List<Resolution>(); 
            for (int i = 0; i < count; i++)
            {
                resolutionList.Add(new Resolution() { Width = (resolution[i] >> 16) & 0xFFFF, Height = resolution[i] & 0xFFFF });
            }

            return resolutionList;

        }

        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_SetResolution(UInt32 dviceNo, UInt32 childDeviceNo, UInt32 width, UInt32 height, out UInt32 errorCode);

        // 解像度設定
        public void SetResolution(Resolution selectedResolution)
        {
            UInt32 errorCode;

            // 指定デバイスの解像度を設定する
            UInt32 ret = PDC_SetResolution(_deviceNo, _childNo, selectedResolution.Width, selectedResolution.Height, out errorCode);
            if (ret == 0)
            {
                var ex = new PdclibException("PDC_SetResolution " + errorCode.ToString());
                throw ex;
            }

            // カラータイプと解像度に合わせてライブ画像のバッファーを準備する
            pBuf = null;
            liveImageSource = null;

            if (_colorType == ColorType.PDC_COLORTYPE_COLOR)
            {
                pBuf = new byte[3 * selectedResolution.Width * selectedResolution.Height];
                liveImageSource = new WriteableBitmap((int)selectedResolution.Width, (int)selectedResolution.Height, dpiX, dpiY, pfColor, null);
            }
            else
            {
                pBuf = new byte[selectedResolution.Width * selectedResolution.Height];
                liveImageSource = new WriteableBitmap((int)selectedResolution.Width, (int)selectedResolution.Height, dpiX, dpiY, pfMono, null);
            }

            rect = new Int32Rect(0, 0, (int)selectedResolution.Width, (int)selectedResolution.Height);
        }

        byte[] pBuf;
        WriteableBitmap liveImageSource;
        Int32Rect rect;


        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_GetLiveImageData(UInt32 deviceNo, UInt32 childDeviceNo, UInt32 bitDepth, byte[] pBuf, out UInt32 errorCode);

        // ライブ画像を取得
        public void GetLiveImageData(Resolution selectedResolution, ColorType colorType)
        {
            UInt32 errorCode;

            // 指定デバイスのライブ画像を取得する
            UInt32 ret = PDC_GetLiveImageData(_deviceNo, _childNo, 8, pBuf, out errorCode);
            if (ret == 0)
            {
                var ex = new PdclibException("PDC_GetLiveImageData " + errorCode.ToString());
                throw ex;
            }
            else
            {
                // 取得した画像をliveImageSourceに格納する
                if (colorType == ColorType.PDC_COLORTYPE_COLOR)
                    liveImageSource.WritePixels(rect, pBuf, (int)selectedResolution.Width * 3, 0);
                else
                    liveImageSource.WritePixels(rect, pBuf, (int)selectedResolution.Width, 0);
            }            
        }


        public BitmapSource LiveImageSource
        {
            get { return liveImageSource; }
        }

        // 拡大率リスト
        List<Magnification> magnificationList = new List<Magnification>() 
        { 
            new Magnification() { ActualValue = 0.1, DisplayValue = "10%" }, 
            new Magnification() { ActualValue = 0.25, DisplayValue = "25%" },
            new Magnification() { ActualValue = 0.5, DisplayValue = "50%" },
            new Magnification() { ActualValue = 1, DisplayValue = "100%" },
            new Magnification() { ActualValue = 1.5, DisplayValue = "150%" },
            new Magnification() { ActualValue = 2, DisplayValue = "200%" },
            new Magnification() { ActualValue = 4, DisplayValue = "400%" },
            new Magnification() { ActualValue = 6, DisplayValue = "600%" },
            new Magnification() { ActualValue = 8, DisplayValue = "800%" },
            new Magnification() { ActualValue = 10, DisplayValue = "1000%" }
        };       

        public List<Magnification> MagnificationList
        {
            get { return magnificationList; }
        }

        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_SetRecReady(UInt32 deviceNo, out UInt32 errorCode);
        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_TriggerIn(UInt32 deviceNo, out UInt32 errorCode);
        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_GetStatus(UInt32 deviceNo, out UInt32 status, out UInt32 errorCode);
        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_SetStatus(UInt32 deviceNo, UInt32 mode, out UInt32 errorCode);
        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_GetMaxFrames (UInt32 deviceNo, UInt32 childDeviceNo, out UInt32 frames, out UInt32 blocks, out UInt32 errorCode);


        public void RecordStart()
        {
            UInt32 status;
            UInt32 errorCode;

            // 録画準備状態にする
            UInt32 ret = PDC_SetRecReady(_deviceNo, out errorCode);
            if (ret == 0)
            {
                var ex = new PdclibException("PDC_SetRecReady " + errorCode.ToString());
                throw ex;
            }

            while (true)
            {
            　　// カメラ録画準備状態の確認
                ret = PDC_GetStatus(_deviceNo, out status, out errorCode);
                if (ret == 0)
                {
                    var ex = new PdclibException("PDC_GetStatus " + errorCode.ToString());
                    throw ex;
                }

                if (status == (uint)CameraStatus.PDC_STATUS_RECREADY || status == (uint)CameraStatus.PDC_STATUS_REC)
                    break;
            }
            Thread.Sleep(10);

            // 録画開始
            ret = PDC_TriggerIn(_deviceNo, out errorCode);
            if (ret == 0)
            {
                var ex = new PdclibException("PDC_TriggerIn " + errorCode.ToString());
                throw ex;
            }


        }

        // 録画状態の取得（UI以外のスレッドで実行）
        public string GetStatus()
        {
            string cameraStatus;
            UInt32 status;
            UInt32 errorCode;

            // カメラの動作状態を取得する
            UInt32 ret = PDC_GetStatus(_deviceNo, out status, out errorCode);
            if (ret == 0)
            {
                var ex = new PdclibException("PDC_GetStatus " + errorCode.ToString());
                throw ex;
            }

            if (status == (uint)CameraStatus.PDC_STATUS_RECREADY)
                cameraStatus = "RECORD READY";
            else if (status == (uint)CameraStatus.PDC_STATUS_REC)
                cameraStatus = "RECORDING";
            else
                cameraStatus = "LIVE";

            return cameraStatus;
            
        }

        
        // 録画停止
        public void RecordStop()
        {
            UInt32 errorCode;

            // 録画を中断して、ライブモードに切り替える
            UInt32 ret = PDC_SetStatus(_deviceNo, (uint)CameraStatus.PDC_STATUS_LIVE, out errorCode);
            if (ret == 0)
            {
                var ex = new PdclibException("PDC_SetStatus " + errorCode.ToString());
                throw ex;
            }
        }
    }

    // 解像度のクラス
    public class Resolution
    {
        UInt32 width;
        UInt32 height;

        public UInt32 Width
        {
            get { return width; }
            set { width = value; }                         
        }

        public UInt32 Height
        {
            get { return height; }
            set { height = value; }           
        }
        public Resolution()
        {

        }

    }

    // 拡大率のクラス
    public class Magnification
    {
        string displayValue;
        double actualValue;

        public string DisplayValue
        {
            get { return displayValue; }
            set { displayValue = value; }
        }

        public double ActualValue
        {
            get { return actualValue; }
            set { actualValue = value; }
        }
    }
}
