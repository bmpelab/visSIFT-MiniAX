using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Livet;
using System.Runtime.InteropServices;
using System.Threading;

namespace MiniPFV.Models
{
    // カメラのオープンとクローズコントロール
    public class OpenCloseCamera : NotificationObject, IOpenCloseCamera
    {
        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_Init(out UInt32 errorCode);
        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_DetectDeviceLV(UInt32 interfaceCode, ref UInt32 detectNo, UInt32 detectNum, UInt32 detectParam, out UInt32 deviceNum, out UInt32 deviceCode, out UInt32 tmpDeviceNo, out UInt32 interfaceCodeOut, out UInt32 errorCode);
        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_OpenDeviceLV(UInt32 deviceCode, UInt32 tmpDeviceNo, UInt32 interfaceCode, out UInt32 deviceNo, out UInt32 errorCode);
        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_CloseDevice(UInt32 deviceNo, out UInt32 errorCode);

        /*
         * NotificationObjectはプロパティ変更通知の仕組みを実装したオブジェクトです。
         */
        private bool isInit = false;
        private bool isOpen = false;

        private UInt32 deviceNo;
        public OpenCloseCamera()
        {
        }

        // カメラを検索する
        public Result DetectCamera(UInt32 IPAdress, out string message)
        {
            message = string.Empty;
            UInt32 ret;
            UInt32 errorCode;

            // カメラを検索する前にtokenを生成する
            cancelTokenSource = new CancellationTokenSource();

            try
            {
                // PDCLIBの初期化
                if (!isInit)
                {
                    ret = PDC_Init(out errorCode);
                    if (ret == 0)
                    {
                        message = "PDC_Init" + errorCode.ToString();
                        return Result.Error;
                    }
                    isInit = true;
                }
         
                cancelTokenSource.Token.ThrowIfCancellationRequested();

                // デバイスの検索
                UInt32 deviceNum;
                UInt32 deviceCode;
                UInt32 tmpDeviceNo;
                UInt32 interfaceCode;
                ret = PDC_DetectDeviceLV(2, ref IPAdress, 1, 0, out deviceNum, out deviceCode, out tmpDeviceNo, out interfaceCode, out errorCode);
                if (ret == 0)
                {
                    message = "PDC_DetectDeviceLV" + errorCode.ToString();
                    return Result.Error;
                }
                cancelTokenSource.Token.ThrowIfCancellationRequested();

                // デバイスのオープン
                ret = PDC_OpenDeviceLV(deviceCode, tmpDeviceNo, interfaceCode, out deviceNo, out errorCode);
                if (ret == 0)
                {
                    message = "PDC_OpenDeviceLV" + errorCode.ToString();
                    return Result.Error;
                }
                isOpen = true;
                cancelTokenSource.Token.ThrowIfCancellationRequested();

                // カメラコントロールの生成
                camera = new ControlCameraBase(deviceNo); 
                if (!camera.Init())
                {
                    message = camera.ErrorMessage;
                    return Result.Error;
                }

                return Result.Completed;
            }

            // カメラ検索をキャンセルする場合
            catch (OperationCanceledException)
            {
                // デバイスすでにオープンしていた場合、クローズする
                if (isOpen)
                {
                    Close();
                }
                return Result.Canceled;
            }
        }

        private CancellationTokenSource cancelTokenSource = new CancellationTokenSource();

        // カメラ検索をキャンセルする
        public void Abort()
        {
            cancelTokenSource.Cancel();
        }

        public void Close()
        {
            UInt32 ret;
            UInt32 errorCode;
            if (isOpen)
            {
                // デバイスをクローズする
                ret = PDC_CloseDevice(deviceNo, out errorCode);
            }
        }

        IControlCamera camera = null;
        public IControlCamera Camera
        {
            get { return camera; }
        }

    }
}
