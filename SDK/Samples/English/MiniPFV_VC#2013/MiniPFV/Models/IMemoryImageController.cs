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
        /// Get information of memory image
        /// </summary>
        void GetMemImageInfo();

        /// <summary>
        /// Get frame information of memory image
        /// </summary>
        PDC_FRAME_INFO FrameInfo { get; }

        /// <summary>
        /// Get memory image
        /// </summary>
        void GetMemImageData(int frameNo);

        /// <summary>
        /// Data of memory image
        /// </summary>
        BitmapSource MemImageSource {get;}

        /// <summary>
        /// Save image file
        /// </summary>
        void SaveFile(string fileName, int startFrameNo, int endFrameNo, Window savingFileWindow);

        /// <summary>
        /// Cancel saving image file
        /// </summary>
        void CancelSaveFile();

        /// <summary>
        /// Get record rate of memory images
        /// </summary>
        UInt32 RecordRate { get; }

        /// <summary>
        /// Get shutter speed of memory images
        /// </summary>
        UInt32 RecordShutterSpeed { get; }

        /// <summary>
        /// Get resolution of memory images
        /// </summary>
        Resolution RecordResolution { get; }

    }
}
