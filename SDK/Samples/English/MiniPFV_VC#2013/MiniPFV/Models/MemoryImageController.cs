using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

using Livet;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows;
using System.IO;
using System.Threading;
using System.Windows.Forms;

namespace MiniPFV.Models
{
    [StructLayout(LayoutKind.Sequential)]

    // Structure for frame information of the recorded partition
    public struct PDC_FRAME_INFO
    {
        public Int32 m_nStart;
        public Int32 m_nEnd;
        public Int32 m_nTrigger;
        public Int32 m_nTwoStageLowToHigh;
        public Int32 m_nTwoStageHighToLow;
        public UInt32 m_nTwoStageTiming;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 10)]
        public Int32[] m_nEvent;
        public UInt32 m_nEventCount;
        public UInt32 m_nRecordedFrames;
    }

    // Image controller of memory mode
    public class MemoryImageController: NotificationObject, IMemoryImageController
    {
        // Save data as 8bit MRAW file
        protected const UInt32 PDC_MRAW_BITDEPTH_8 = 0;

        // Pixel formats of bitmap image
        protected PixelFormat pfColor = PixelFormats.Bgr24;
        protected PixelFormat pfMono = PixelFormats.Gray8;

        // dpi value of bitmap image
        protected const double dpiX = 96;
        protected const double dpiY = 96;

        UInt32 _deviceNo;
        UInt32 _childDeviceNo;
        ColorType _colorType;
        string _deviceName = null;

        public MemoryImageController(UInt32 deviceNo, UInt32 childNo, ColorType colorType, string deviceName)
        {
            _deviceNo = deviceNo;
            _childDeviceNo = childNo;
            _colorType = colorType;
            _deviceName = deviceName;
        }


        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_GetMemFrameInfo(UInt32 deviceNo, UInt32 childDeviceNo, out PDC_FRAME_INFO frameInfo, out UInt32 errorCode);
        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_GetMemImageData(UInt32 deviceNo, UInt32 childDeviceNo, Int32 frameNo, UInt32 bitDepth, byte[] pBuf, out UInt32 errorCode);
        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_GetMemResolution(UInt32 deviceNo, UInt32 childDeviceNo, out UInt32 width, out UInt32 height, out UInt32 errorCode);
        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_GetMemShutterSpeedFps(UInt32 deviceNo, UInt32 childDeviceNo, out UInt32 shutterSpeed, out UInt32 errorCode);
        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_GetMemRecordRate(UInt32 deviceNo, UInt32 childDeviceNo, out UInt32 recordRate, out UInt32 errorCode);
        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_GetMemColorTempMode(UInt32 deviceNo, UInt32 childDeviceNo, out UInt32 colorTempMode, out UInt32 colorBalanceR, out UInt32 colorBalanceG, out UInt32 colorBalanceB,  out UInt32 errorCode);
        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_GetMemBitDepth(UInt32 deviceNo, UInt32 childDeviceNo, out char bitDepth, out UInt32 errorCode);

        byte[] pBuf;
        Int32Rect rect;

        // Get information of memory image
        public void GetMemImageInfo()
        {
            UInt32 errorCode;

            // Get frame information of images in current recorded partition
            UInt32 ret = PDC_GetMemFrameInfo(_deviceNo, _childDeviceNo, out frameInfo, out errorCode);
            if (ret == 0)
            {
                var ex = new PdclibException("PDC_GetMemFrameInfo " + errorCode.ToString());
                throw ex;
            }

            UInt32 width;
            UInt32 height;
            // Get resolution of images in current recorded partition
            ret = PDC_GetMemResolution(_deviceNo, _childDeviceNo, out width, out height, out errorCode);
            if (ret == 0)
            {
                var ex = new PdclibException("PDC_GetMemResolution " + errorCode.ToString());
                throw ex;
            }
            recordResolution = new Resolution() { Width = width, Height = height };

            // Get record rate of images in current recorded partition
            ret = PDC_GetMemRecordRate(_deviceNo, _childDeviceNo, out recordRate, out errorCode);
            if (ret == 0)
            {
                var ex = new PdclibException("PDC_GetMemRecordRate " + errorCode.ToString());
                throw ex;
            }

            // Get shutter speed of images in current recorded partition
            ret = PDC_GetMemShutterSpeedFps(_deviceNo, _childDeviceNo, out recordShutterSpeed, out errorCode);
            if (ret == 0)
            {
                var ex = new PdclibException("PDC_GetMemShutterSpeedFps " + errorCode.ToString());
                throw ex;
            }

            // Prepare image buffer according to color type and resolution
            if (_colorType == ColorType.PDC_COLORTYPE_COLOR)
            {
                pBuf = new byte[width * height * 3];
                memImageSource = new WriteableBitmap((int)width, (int)height, dpiX, dpiY, pfColor, null);
            }
            else
            {
                pBuf = new byte[width * height];
                memImageSource = new WriteableBitmap((int)width, (int)height, dpiX, dpiY, pfMono, null);
            }

            rect = new Int32Rect(0, 0, (int)width, (int)height);
        }

    　　// Image information on play screen-------------------
        PDC_FRAME_INFO frameInfo;
        public PDC_FRAME_INFO FrameInfo
        {
            get { return frameInfo; }
        }

        UInt32 recordRate;
        public UInt32 RecordRate
        {
            get { return recordRate; }
        }

        UInt32 recordShutterSpeed;
        public UInt32 RecordShutterSpeed
        {
            get { return recordShutterSpeed; }
        }

        Resolution recordResolution;
        public Resolution RecordResolution
        {
            get { return recordResolution; }
        }
        //----------------------------------------------


        // Get image data for playback (Execute in different thread from UI)
        public void GetMemImageData(int frameNo)
        {
            UInt32 errorCode;

            // Get recorded image in current partition
            UInt32 ret = PDC_GetMemImageData(_deviceNo, _childDeviceNo, frameNo, 8, pBuf, out errorCode);
            if (ret == 0)
            {
                var ex = new PdclibException("PDC_GetMemImageData " + errorCode.ToString());
                throw ex;
            }
            else
            {
                // Save image data to memImageSource
                if (_colorType == ColorType.PDC_COLORTYPE_COLOR)
                    memImageSource.WritePixels(rect, pBuf, (int)recordResolution.Width * 3, 0);
                else
                    memImageSource.WritePixels(rect, pBuf, (int)recordResolution.Width, 0);

            }
        }

        WriteableBitmap memImageSource;
        public BitmapSource MemImageSource
        {
            get {return memImageSource;}
        }

        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_MRAWFileSaveOpen(UInt32 deviceNo, UInt32 childDeviceNo, string lpszFileName, UInt32 mrawBitDepth, Int32 maxFrameNum, out UInt32 errorCode);
        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_MRAWFileSave(UInt32 deviceNo, UInt32 childDeviceNo, Int32 frameNo, out UInt32 errorCode);
        [DllImport("pdclib.dll")]
        static extern UInt32 PDC_MRAWFileSaveClose(UInt32 deviceNo, UInt32 childDeviceNo, out UInt32 errorCode);


        private CancellationTokenSource cancelTokenSource = new CancellationTokenSource();

        // Save image file
        public void SaveFile(string fileName, int startFrameNo, int endFrameNo, Window savingFileWindow)
        {

            object[] obj = new object[3];

            obj[0] = fileName;
            obj[1] = startFrameNo;
            obj[2] = endFrameNo;

            // Renew before image saving
            cancelTokenSource = new CancellationTokenSource();

            // Execute in other thread 
            Task.Factory.StartNew(() =>
                {
                 MRAWFileSave(obj, cancelTokenSource.Token, savingFileWindow);
                });
        }

        // Cancel file saving
        public void CancelSaveFile()
        {
            cancelTokenSource.Cancel();
        }


        private void MRAWFileSaveOpen(string mrawFileName)
        {            
            UInt32 errorCode;

            // Initialization of MRAW file saving 
            UInt32 ret = PDC_MRAWFileSaveOpen(_deviceNo, _childDeviceNo, mrawFileName, PDC_MRAW_BITDEPTH_8, (int)frameInfo.m_nRecordedFrames, out errorCode);
            if (ret == 0)
            {
                // Throw PdclibException when error occures
                var exSaveOpen = new PdclibException("PDC_MRAWFileSaveOpen Error " + errorCode.ToString());
                throw exSaveOpen;
            }
            // Throw OperationCancelException when file saving is canceled
            cancelTokenSource.Token.ThrowIfCancellationRequested();
        }

        private void MRAWFileSave(int startFrameNo, int endFrameNo)
        {
            UInt32 errorCode;
            UInt32 ret;
            for (int i = startFrameNo; i <= endFrameNo; i++)
            {
                // Save designated frames to MRAW file
                ret = PDC_MRAWFileSave(_deviceNo, _childDeviceNo, i, out errorCode);
                if (ret == 0)
                {
                    // Throw PdclibException when error occures
                    var exSave = new PdclibException("PDC_MRAWFileSave Error" + errorCode.ToString());
                    throw exSave;
                }
                cancelTokenSource.Token.ThrowIfCancellationRequested();              
            }

        }

        private void MRAWFileSaveClose()
        {
            UInt32 errorCode;

            // Finish MRAW file saving
            UInt32 ret = PDC_MRAWFileSaveClose(_deviceNo, _childDeviceNo, out errorCode);
            if (ret == 0)
            {
                // Throw PdclibException when error occures
                var exSaveClose = new PdclibException("PDC_MRAWFileSaveClose Error" + errorCode.ToString());
                throw exSaveClose;
            }

            cancelTokenSource.Token.ThrowIfCancellationRequested();
        }

        // Save CIH file
        private void cihFileSave(string cihFileName, int startFrameNo, int endFrameNo, Window savingFileWindow)
        {
            UInt32 errorCode;
            UInt32 ret;

            UInt32 colorTempMode;
            UInt32 colorBalanceR;
            UInt32 colorBalanceG;
            UInt32 colorBalanceB;
            char bitDepth;

            StreamWriter cihWriter = new StreamWriter(cihFileName);

            cihWriter.WriteLine("#Camera Information Header");

            cihWriter.Write("Camera Type :");
            cihWriter.WriteLine(_deviceName);

            cihWriter.Write("Camera Number :");
            cihWriter.WriteLine(_deviceNo);

            cihWriter.Write("Head Number :");
            cihWriter.WriteLine(_childDeviceNo);

            cihWriter.Write("Record Rate(fps) :");
            cihWriter.WriteLine(recordRate);

            cihWriter.Write("Shutter Speed(s) :");
            cihWriter.WriteLine("1/" + recordShutterSpeed);

            cihWriter.Write("Trigger Mode :");
            cihWriter.WriteLine("Start");

            if (_colorType == ColorType.PDC_COLORTYPE_COLOR)
            {
                ret = PDC_GetMemColorTempMode(_deviceNo, _childDeviceNo, out colorTempMode, out colorBalanceR, out colorBalanceG, out colorBalanceB, out errorCode);
                if (ret == 0)
                {
                    var ex = new PdclibException("PDC_GetMemColorTempMode " + errorCode.ToString());
                    throw ex;
                }
                cihWriter.Write("Color Temperature :");
                if (colorTempMode == 1)
                    cihWriter.WriteLine("5100K");
                else if (colorTempMode == 2)
                    cihWriter.WriteLine("3100K");
                else
                    cihWriter.WriteLine("");

                cihWriter.Write("Color Balance R :");
                cihWriter.WriteLine(colorBalanceR);

                cihWriter.Write("Color Balance G :");
                cihWriter.WriteLine(colorBalanceG);

                cihWriter.Write("Color Balance B :");
                cihWriter.WriteLine(colorBalanceB);
            }


            cihWriter.Write("Original Total Frame :");
            cihWriter.WriteLine(frameInfo.m_nRecordedFrames);

            cihWriter.Write("Total Frame :");
            cihWriter.WriteLine(endFrameNo - startFrameNo + 1);

            cihWriter.Write("Start Frame :");
            cihWriter.WriteLine(startFrameNo);

            cihWriter.Write("Image Width :");
            cihWriter.WriteLine(recordResolution.Width);

            cihWriter.Write("Image Height :");
            cihWriter.WriteLine(recordResolution.Height);

            cihWriter.Write("Color Type :");
            if (_colorType == ColorType.PDC_COLORTYPE_COLOR)
                cihWriter.WriteLine("Color");
            else
                cihWriter.WriteLine("Mono");

            cihWriter.Write("Color Bit :");
            if (_colorType == ColorType.PDC_COLORTYPE_COLOR)
                cihWriter.WriteLine("24");
            else
                cihWriter.WriteLine("8");

            cihWriter.Write("File Format :");
            cihWriter.WriteLine("MRaw");

            ret = PDC_GetMemBitDepth(_deviceNo, _childDeviceNo, out bitDepth, out errorCode);
            if (ret == 0)
            {
                var ex = new PdclibException("PDC_GetMemBitDepth " + errorCode.ToString());
                throw ex;
            }
            cihWriter.Write("EffectiveBit Depth :");
            cihWriter.WriteLine((int)bitDepth);

            cihWriter.Write("EffectiveBit Side :");
            cihWriter.WriteLine("Lower");

            cihWriter.WriteLine("Zero Frame : Exist");

            cihWriter.WriteLine("END");

            cihWriter.Close();

            cancelTokenSource.Token.ThrowIfCancellationRequested();
          
        }

        // Save MRAW images (Execute in different thread from UI)
        public void MRAWFileSave(object obj, CancellationToken token, Window savingFileWindow)
        {
            object[] objTmp = (object[])obj;
            string mrawFileName;
            string cihFileName;

            // Convert the extention of file name to .cih
            string fileName = objTmp[0].ToString();
            mrawFileName = fileName;
            cihFileName = fileName.Remove(fileName.Length - 4) + "cih";

            try
            {
                MRAWFileSaveOpen(mrawFileName);
                MRAWFileSave((int)objTmp[1], (int)objTmp[2]);
                MRAWFileSaveClose();


                try
                {
                    cihFileSave(cihFileName, (int)objTmp[1], (int)objTmp[2], savingFileWindow);

                    // Close savingFileWindow when file saving is finished
                    DispatcherHelper.UIDispatcher.Invoke(() =>
                    {
                        savingFileWindow.Hide();
                    });
                    System.Windows.MessageBox.Show("Image saving is successed!", "", MessageBoxButton.OK);

                }
                // Exception of PDCLIB function error
                catch (PdclibException ex)
                {
                    DispatcherHelper.UIDispatcher.Invoke(() =>
                    {
                        savingFileWindow.Hide();
                    });

                    System.Windows.MessageBox.Show(ex.Message, "Image saving is failed!", MessageBoxButton.OK);

                    // Delete MRAW file already created
                    File.Delete(mrawFileName);

                }

                // Exception of file saving cancellation
                catch (OperationCanceledException)
                {
                    DispatcherHelper.UIDispatcher.Invoke(() =>
                    {
                        savingFileWindow.Hide();
                    });

                    System.Windows.MessageBox.Show("Image saving is canceled.", " ", MessageBoxButton.OK);

                    // Delete MRAW file and CIH file already created
                    File.Delete(mrawFileName);
                    File.Delete(cihFileName);

                }
            }
            // Exception of PDCLIB function error
            catch (PdclibException ex)
            {
                DispatcherHelper.UIDispatcher.Invoke(() =>
                {
                    savingFileWindow.Hide();
                });

                System.Windows.MessageBox.Show(ex.Message, "Image saving is failed!", MessageBoxButton.OK);

                // Delete MRAW file already created
                UInt32 errorCode;
                PDC_MRAWFileSaveClose(_deviceNo, _childDeviceNo, out errorCode);
                File.Delete(mrawFileName);

            }
            // Exception of file saving cancellation
            catch (OperationCanceledException)
            {
                DispatcherHelper.UIDispatcher.Invoke(() =>
                {
                    savingFileWindow.Hide();
                });

                System.Windows.MessageBox.Show("Image saving is canceled.", " ", MessageBoxButton.OK);

                UInt32 errorCode;
                PDC_MRAWFileSaveClose(_deviceNo, _childDeviceNo, out errorCode);
                File.Delete(mrawFileName);

            }
          
        }


    }

    public class PdclibException : ApplicationException
    {
        public PdclibException() { }
        public PdclibException(string message) : base(message) { }
        public PdclibException(string message, Exception inner) : base(message, inner) { }
    }
}
