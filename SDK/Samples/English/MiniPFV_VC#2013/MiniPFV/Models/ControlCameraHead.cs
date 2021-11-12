using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Livet;
using System.Collections.ObjectModel;
using System.Runtime.InteropServices;
using System.Windows.Threading;
using System.Windows.Media;

namespace MiniPFV.Models
{
    public class ControlCameraHead : NotificationObject, IControlCameraHead
    {
        private readonly UInt32 _deviceNo;
        private readonly UInt32 _childNo;

        public ControlCameraHead(UInt32 deviceNo, UInt32 childNo)
        {
            _deviceNo = deviceNo;
            _childNo = childNo;
        }  
          
        public string Name
        {
            get { return "Head No." + _childNo.ToString(); }
        }

        public UInt32 DeviceNo
        {
            get { return _deviceNo; }
        }

        public UInt32 ChildNo
        {
            get { return _childNo; }
        }

        protected string errorMessage;
        public string ErrorMessage
        {
            get
            {
                return errorMessage;
            }
        }

        public const UInt32 PDC_MAX_STRING_LENGTH = 256;

        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_GetDeviceName(UInt32 deviceNo, UInt32 childNo, ref byte cameraName, out UInt32 errorCode);

        public string DeviceName
        {
            get{        
                byte[] strName = new byte[PDC_MAX_STRING_LENGTH];
                string deviceName;

                UInt32 errorCode;

                // Get the name of designated device
                UInt32 ret = PDC_GetDeviceName(_deviceNo, 0, ref strName[0], out errorCode);
                if (ret == 0)
                {
                    errorMessage = "PDC_GetDeviceName " + errorCode.ToString();
                }
                deviceName = System.Text.Encoding.UTF8.GetString(strName).Replace("\0", null);

                return deviceName;
            }
        }


        [DllImport("PDCLIB.dll")]
        static extern UInt32 PDC_GetColorType(UInt32 deviceNo, UInt32 childNo, out char colorMode, out UInt32 errorCode);

        // Confirm color type of camera head
        public ColorType ColorType
        {
            get {
                char colorType;
                UInt32 errorCode;

                // Get color type of the designated device
                UInt32 ret = PDC_GetColorType(_deviceNo, _childNo, out colorType, out errorCode);
                if (ret == 0)
                {
                    errorMessage = "PDC_GetColorType " + errorCode.ToString();
                }

                return (ColorType)colorType; }
        }        
        


    }
}
