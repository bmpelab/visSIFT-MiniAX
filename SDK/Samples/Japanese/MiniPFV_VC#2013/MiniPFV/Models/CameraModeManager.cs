using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Runtime.InteropServices;


namespace MiniPFV.Models
{

    public class CameraModeManager
    {
        public CameraModeManager(UInt32 deviceNo, UInt32 mode)
        {
            _deviceNo = deviceNo;
            _mode = mode;
        }

        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_SetStatus(UInt32 deviceNo, UInt32 mode, out UInt32 errorCode);

        UInt32 _deviceNo;
        UInt32 _mode;

        public void SetStatus()
        {
            UInt32 ret;
            UInt32 errorCode;

            // カメラモードを設定する
            ret = PDC_SetStatus(_deviceNo, _mode, out errorCode);
            if (ret == 0)
            {
                var ex = new PdclibException("PDC_SetStatus " + errorCode.ToString());
                throw ex;
            }
        }

        
    }
}
