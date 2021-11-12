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
        /// Get record rate list
        /// </summary>
        IList<UInt32> GetRecordRateList();

        /// <summary>
        /// Set record rate
        /// </summary>
        void SetRecordRate(UInt32 selectedRecordRate);

        /// <summary>
        /// Get shutter speed list
        /// </summary>
        IList<UInt32> GetShutterSpeedList();


        /// <summary>
        /// Set shutter speed 
        /// </summary>
        void SetShutterSpeed(UInt32 selectedShutterSpeed);

        /// <summary>
        /// Get resolution list
        /// </summary>
        IList<Resolution> GetResolutionList();

        /// <summary>
        /// Set resolution
        /// </summary>
        void SetResolution(Resolution selectedResolution);

        /// <summary>
        /// Data of live image
        /// </summary>
        BitmapSource LiveImageSource { get; }

        /// <summary>
        /// Get live image
        /// </summary>
        void GetLiveImageData(Resolution selectedResolution, ColorType colorType);

        /// <summary>
        /// Get magnification list
        /// </summary>
        List<Magnification> MagnificationList { get; }

        /// <summary>
        /// Start to record
        /// </summary>
        void RecordStart();

        /// <summary>
        /// Stop recording
        /// </summary>
        void RecordStop();

        /// <summary>
        /// Get recording status
        /// </summary>
        /// <returns>recording status:"LIVE", "RECORD READY", "RECORDING"</returns>
        string GetStatus();

    }
}
