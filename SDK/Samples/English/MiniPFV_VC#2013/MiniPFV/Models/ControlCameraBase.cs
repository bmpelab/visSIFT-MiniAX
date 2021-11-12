using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Livet;
using System.Runtime.InteropServices;

namespace MiniPFV.Models
{
    public enum ColorType
    {
        PDC_COLORTYPE_MONO = 0,
        PDC_COLORTYPE_COLOR = 1,
    };

    public class ControlCameraBase : NotificationObject, IControlCamera
    {
        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_SetStatus(UInt32 deviceNo, UInt32 mode, out UInt32 errorCode);
        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_GetExistChildDeviceList(UInt32 deviceNo, out UInt32 childCount, IntPtr list, out UInt32 errorCode);


        protected readonly UInt32 _deviceNo;
        public ControlCameraBase(UInt32 deviceNo)
        {
            _deviceNo = deviceNo;       
        }

        protected string errorMessage;
        public string ErrorMessage
        {
            get { return errorMessage; }
        }

        public bool Init()
        {
            UInt32 ret;
            UInt32 errorCode;
            // Set camera to live mode
            ret = PDC_SetStatus(_deviceNo, 0, out errorCode);
            if (ret == 0)
            {
                errorMessage = "PDC_SetStatus" + errorCode.ToString();
                return false;
            }

            // Confirm the connected camera heads
            UInt32 childCount;
            IntPtr list = Marshal.AllocHGlobal(64);
            ret = PDC_GetExistChildDeviceList(_deviceNo, out childCount, list, out errorCode);
            if (ret == 0)
            {
                errorMessage = "PDC_GetExistChildDeviceList" + errorCode.ToString();
                return false;
            }
            int[] childList = new int[childCount];
            Marshal.Copy(list, childList, 0, (int)childCount);

            // Make camera head list
            for (int i = 0; i < childCount; i++)
            {
                cameraHeads.Add(new ControlCameraHead(_deviceNo, (UInt32)childList[i]));
                errorMessage = cameraHeads[i].ErrorMessage;
            }
            Marshal.FreeHGlobal(list);

            return true;
        }

        IList<IControlCameraHead> cameraHeads = new List<IControlCameraHead>();
        public IList<IControlCameraHead> CameraHeads
        {
            get { return cameraHeads; }
        }



    }
}
