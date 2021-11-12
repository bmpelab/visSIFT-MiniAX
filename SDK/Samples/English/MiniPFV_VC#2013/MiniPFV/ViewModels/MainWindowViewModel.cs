using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

using Livet;
using Livet.Commands;
using Livet.Messaging;
using Livet.Messaging.IO;
using Livet.EventListeners;
using Livet.Messaging.Windows;

using MiniPFV.Models;
using System.Threading.Tasks;
using System.Windows.Threading;

using System.Runtime.InteropServices;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.IO;

using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Forms;

namespace MiniPFV.ViewModels
{
    public class MainWindowViewModel : ViewModel
    {
        

        public const UInt32 LIVE_MODE = 0;
        public const UInt32 MEMORY_MODE = 1;


        public void Initialize()
        {
        }
        
        private readonly IControlCamera _control;
        ILiveImageController _controlLiveImage;
        IMemoryImageController _controlMemImage;

        public MainWindowViewModel(IControlCamera control)
        {
            _control = control;
            if (_control.CameraHeads.Count > 0)
            {
                // Choose the first camera head in head list
                selectedCameraHead = _control.CameraHeads[0];
                
                // Create controller of live image and memory image
                _controlLiveImage = new LiveImageController(selectedCameraHead.DeviceNo, selectedCameraHead.ChildNo, selectedCameraHead.ColorType);
                _controlMemImage = new MemoryImageController(selectedCameraHead.DeviceNo, selectedCameraHead.ChildNo, selectedCameraHead.ColorType, selectedCameraHead.DeviceName);
                
                // Set initial magnification to 100%
                SelectedMagnification = _controlLiveImage.MagnificationList[3];

                // Set initial record rate of camera
                rateList = _controlLiveImage.GetRecordRateList();
                SelectedFrameRate = rateList[0];

                // Get live image in other thread
                Task.Factory.StartNew(RealtimeLoadImage);
            }
            else
            {
                errorMessage = "No Camera Head.";
            }
        }



        public string CameraName
        {
            get {            

                return selectedCameraHead.DeviceName;

            }
        }

        string errorMessage;
        public string ErrorMessage
        {
            get { return errorMessage; }
        }


        public IList<IControlCameraHead> CameraHeads
        {
            get { return _control.CameraHeads; }
        }

        IControlCameraHead selectedCameraHead;

        public IControlCameraHead SelectedCameraHead
        {
            get { return selectedCameraHead; }
            set 
            {
                selectedCameraHead = value;
            }
        }

        IList<UInt32> rateList;
        public IList<UInt32> RateList
        {
            get {
                try
                {
                    // Get record rate list 
                    rateList = _controlLiveImage.GetRecordRateList();
                }
                catch (PdclibException ex)
                {
                    Messenger.Raise(new InformationMessage("Error: " + ex.Message + "\n" + "Get Frame rate list failed.", "", MessageBoxImage.Error, "Information"));                  
                }
                return rateList; }
          
        }

        UInt32 selectedFrameRate;
        public UInt32 SelectedFrameRate
        {
            get { return selectedFrameRate; }
            set
            {
                selectedFrameRate = value;
                try
                {
                    // Set to the designated record rate
                    _controlLiveImage.SetRecordRate(selectedFrameRate);

                    // Update shutter speed list and resolution list according to the designated record rate 
                    RaisePropertyChanged(() => ShutterSpeedList);
                    RaisePropertyChanged(() => ResolutionList);

                    // Set to the first value of each list 
                    SelectedShutterSpeed = ShutterSpeedList[0];
                    RaisePropertyChanged(() => SelectedShutterSpeed);
                    SelectedResolution = ResolutionList[0];
                    RaisePropertyChanged(() => SelectedResolution);
                }
                catch (PdclibException ex)
                {
                    Messenger.Raise(new InformationMessage("Error: " + ex.Message + "\n" + "Frame rate set failed." , "", MessageBoxImage.Error, "Information"));

                }

            }        
        }

        IList<UInt32> shutterSpeedList;
        public IList<UInt32> ShutterSpeedList
        {
            get {
                try
                {
                    // Get shutter speed list 
                    shutterSpeedList = _controlLiveImage.GetShutterSpeedList();
                }
                catch (PdclibException ex)
                {
                    Messenger.Raise(new InformationMessage("Error: " + ex.Message + "\n" + "Get shutter speed list failed.", "", MessageBoxImage.Error, "Information"));
                }
                return shutterSpeedList;
            }

       }

        UInt32 selectedShutterSpeed;
        public UInt32 SelectedShutterSpeed
        {
            get { return selectedShutterSpeed; }
            set
            {
                selectedShutterSpeed = value;
                try
                {
                    // Set to the designated shutter speed 
                    _controlLiveImage.SetShutterSpeed(selectedShutterSpeed);
                }
                catch (PdclibException ex)
                {
                    Messenger.Raise(new InformationMessage("Error: " + ex.Message + "\n" + "Set shutter speed failed.", "", MessageBoxImage.Error, "Information"));
                }
            }
        }

        IList<Resolution> resolutionList;
        public IList<Resolution> ResolutionList
        {
            get
            {
                try
                {
                    // Get resolution list 
                    resolutionList = _controlLiveImage.GetResolutionList();
                }
                catch (PdclibException ex)
                {
                        Messenger.Raise(new InformationMessage("Error: " + ex.Message + "\n" + "Get resolution list failed.", "", MessageBoxImage.Error, "Information"));

                }
                return resolutionList;
            }
        }

        Resolution selectedResolution;
        public Resolution SelectedResolution
        {
            get
            {
                return selectedResolution;
            }
            set
            {
               selectedResolution = value;
               try
                {
                    // Set to the designated resolution 
                    _controlLiveImage.SetResolution(selectedResolution);
                }
                catch (PdclibException ex)
                {
                    Messenger.Raise(new InformationMessage("Error: " + ex.Message + "\n" + "Set resolution failed.", "", MessageBoxImage.Error, "Information"));
                }

            }
        }

        public BitmapSource LiveImageSource
        {
            get { return _controlLiveImage.LiveImageSource; }
        }


        public bool IsWindowClosed = false;
        public void RealtimeLoadImage()
        {
            // Get live image when camera is set to live mode and the window is not closed 
            while (SelectedTab == LIVE_MODE && IsWindowClosed == false)
            {
                // Execute in UI thread 
                DispatcherHelper.UIDispatcher.Invoke(() =>
                    {
                        try
                        {
                            _controlLiveImage.GetLiveImageData(SelectedResolution, selectedCameraHead.ColorType);
                            RaisePropertyChanged(() => LiveImageSource);
                        }
                        catch (PdclibException ex)
                        {
                            // Show error message when camera is in live mode and the window is not closed 
                            if (SelectedTab == LIVE_MODE && IsWindowClosed == false)
                            {
                                Messenger.Raise(new InformationMessage("Error: " + ex.Message + "\n" + "Get live image failed.", "", MessageBoxImage.Error, "Information"));

                            }
                        }
                    });
                Thread.Sleep(1);
            }

        }


        UInt32 selectedTab = LIVE_MODE;
        UInt32 cameraMode;
        public UInt32 SelectedTab
        {
            get { return selectedTab; }
            set
            {
                selectedTab = value;
                // Convert camera mode according to the selected tab 
                cameraMode = selectedTab;
                try
                {
                    SetCameraStatus(cameraMode);
                }
                catch (PdclibException ex)
                {
                    Messenger.Raise(new InformationMessage("Error: " + ex.Message + "\n" + "Convert camera mode failed.", "", MessageBoxImage.Error, "Information"));
                }
            }
        }


        // Set to initial status when camera mode is changed 
        void SetCameraStatus(UInt32 cameraMode)
        {
            CameraModeManager modeManager = new CameraModeManager(selectedCameraHead.DeviceNo, cameraMode);
            modeManager.SetStatus();

            // Live mode
            if (cameraMode == LIVE_MODE)
            {
                // Show live image 
                Task.Factory.StartNew(RealtimeLoadImage);
            }
            // Memory mode
            else
            {
                // Update information of memory iamges  
                _controlMemImage.GetMemImageInfo();
                RaisePropertyChanged(() => TotalFrameNo);
                RaisePropertyChanged(() => RecordRate);
                RaisePropertyChanged(() => RecordShutterSpeed);
                RaisePropertyChanged(() => RecordResolution);

                // Show the first frame of memory images in initial status 
                _controlMemImage.GetMemImageData(_controlMemImage.FrameInfo.m_nStart);
                RaisePropertyChanged(() => MemImageSource);

                startFrameNo = (UInt32)_controlMemImage.FrameInfo.m_nStart;
                RaisePropertyChanged(() => StartFrameNo);

                currentFrameNo = (UInt32)_controlMemImage.FrameInfo.m_nStart;
                RaisePropertyChanged(() => CurrentFrameNo);

                endFrameNo = (UInt32)_controlMemImage.FrameInfo.m_nEnd;
                RaisePropertyChanged(() => EndFrameNo);

                // Set the range of file saving to all frames in initial status
                saveFileStartFrameNo = _controlMemImage.FrameInfo.m_nStart;
                RaisePropertyChanged(() => SaveFileStartFrameNo);

                saveFileEndFrameNo = _controlMemImage.FrameInfo.m_nEnd;
                RaisePropertyChanged(() => SaveFileEndFrameNo);

            }

        }

        public UInt32 RecordRate
        {
            get { return _controlMemImage.RecordRate; }
        }

        public UInt32 RecordShutterSpeed
        {
            get { return _controlMemImage.RecordShutterSpeed; }
        }

        public Resolution RecordResolution
        {
            get { return _controlMemImage.RecordResolution; }
        }


        public List<Magnification> MagnificationList
        {
            get { return _controlLiveImage.MagnificationList; }
        }

        Magnification selectedMagnification;
        public Magnification SelectedMagnification
        {
            get { return selectedMagnification; }
            set
            {
                selectedMagnification = value;
                RaisePropertyChanged(() => SelectedMagnification);

            }
        }


        bool isRecordStopAvailable = false;
        bool isRecordStartAvailable = true;
        public void RecordStart()
        {
            try
            {
                // Record Stop button is enable and Record Start button is disable when recording starts
                isRecordStopAvailable = true;
                RaisePropertyChanged(() => IsRecordStopAvailable);
                isRecordStartAvailable = false;
                RaisePropertyChanged(() => IsRecordStartAvailable);

                _controlLiveImage.RecordStart();
            }
            catch (PdclibException ex)
            {
                Messenger.Raise(new InformationMessage("Error: " + ex.Message + "\n" + "Can not start recording!", "", MessageBoxImage.Error, "Information"));
            }
            // Check recording status in other thread
            Task.Factory.StartNew(GetStatus);
 
        }

        public void GetStatus()
        {
            try
            {
                while (true)
                {
                    // Get recording status
                    cameraStatus = _controlLiveImage.GetStatus();
                    // Update recording status in UI thread 
                    DispatcherHelper.UIDispatcher.Invoke(() =>
                    {
                        RaisePropertyChanged(() => CameraStatus);
                    });

                    if (cameraStatus == "LIVE")
                        break;

                    Thread.Sleep(1);
                }

                // Convert to memory mode when recording is finished (Automatic finish)
                DispatcherHelper.UIDispatcher.Invoke(() =>
                    {
                        selectedTab = MEMORY_MODE;
                        RaisePropertyChanged(() => SelectedTab);
                        cameraMode = selectedTab;
                        SetCameraStatus(cameraMode);

                        // Recover Record Start button and Record Stop button to initial status
                        isRecordStopAvailable = false;
                        RaisePropertyChanged(() => IsRecordStopAvailable);
                        isRecordStartAvailable = true;
                        RaisePropertyChanged(() => IsRecordStartAvailable);

                    });

            }
            catch (PdclibException ex)
            {
                Messenger.Raise(new InformationMessage("Error: " + ex.Message + "\n" + "Can not get camera status.", "", MessageBoxImage.Error, "Information"));
            }

        }

        public void RecordStop()
        {
            try
            {
                isRecordStopAvailable = false;
                RaisePropertyChanged(() => IsRecordStopAvailable);
                isRecordStartAvailable = true;
                RaisePropertyChanged(() => IsRecordStartAvailable);

                _controlLiveImage.RecordStop();

                // Convert to memory mode when recording is stopped
                selectedTab = MEMORY_MODE;
                RaisePropertyChanged(() => SelectedTab);
                cameraMode = selectedTab;
                SetCameraStatus(cameraMode);
                
            }
            catch (PdclibException ex)
            {
                Messenger.Raise(new InformationMessage("Error: " + ex.Message + "\n" + "Can not stop recording.", "", MessageBoxImage.Error, "Information"));
            }

        }

        public bool IsRecordStopAvailable
        {
            get { return isRecordStopAvailable; }
            set { isRecordStopAvailable = value;}
        }

        public bool IsRecordStartAvailable
        {
            get { return isRecordStartAvailable; }
            set { isRecordStartAvailable = value; }
        }

        string cameraStatus = "LIVE";
        public string CameraStatus
        {
            get {
                // Disable record rate, shutter speed and resolution setting function in recording
                if (cameraStatus != "LIVE")
                    isSettingEnable = false;
                else
                    isSettingEnable = true;
                RaisePropertyChanged(() => IsSettingEnable);
                return cameraStatus; }
        }

        bool isSettingEnable = true;
        public bool IsSettingEnable
        {
            get { return isSettingEnable; }
        }

        public void GetMemImage()
        {            
            // Execute in other thread 
            Task.Factory.StartNew(GetMemImageData);
        }

        UInt32 startFrameNo;
        UInt32 currentFrameNo;
        UInt32 endFrameNo;

        public void GetMemImageData()
        {
            isStop = false;
            // Start playback from Start Frame when Current Frame is out of playback range 
            if (currentFrameNo < startFrameNo || currentFrameNo > endFrameNo)
            {
                currentFrameNo = startFrameNo;
            }

            // Start playback from Current Frame 
            for (int i = (int)currentFrameNo; i <= endFrameNo; i++)
            {
                // Update memory image in UI thread
                DispatcherHelper.UIDispatcher.Invoke(() =>
                       {
                           try
                           {
                               _controlMemImage.GetMemImageData(i);
                               RaisePropertyChanged(() => MemImageSource);
                               RaisePropertyChanged(() => CurrentFrameNo);
                           }
                           catch (PdclibException ex)
                           {
                               Messenger.Raise(new InformationMessage("Error: " + ex.Message + "\n" + "Can not get memory images.", "", MessageBoxImage.Error, "Information"));
                               isStop = true;
                           }
                       });
                currentFrameNo = (UInt32)i+1;
                Thread.Sleep(10);
                // Break when playback is stopped or paused 
                if (isStop)
                    break;
            }
            isStop = true;

        }

        bool isStop = false;
        // Stop playback
        public void StopPlayingMemImages()
        {
            if (!isStop)
            {
                isStop = true;
            }    

            // Return to Start Frame of playback 
            try
            {
                _controlMemImage.GetMemImageData((int)startFrameNo);
                currentFrameNo = startFrameNo;
                RaisePropertyChanged(() => MemImageSource);
                RaisePropertyChanged(() => CurrentFrameNo);
            }
            catch (PdclibException ex)
            {
                Messenger.Raise(new InformationMessage("Error: " + ex.Message + "\n" + "Can not stop playing.", "", MessageBoxImage.Error, "Information"));
            }
            
        }

        // Playback pause
        public void PausePlayingMemImages()
        {
            if (!isStop)
            {
                isStop = true;
            }
        }

        // Current frame
        public UInt32 CurrentFrameNo
        {
            get { return currentFrameNo; }
            set {
                try
                {
                    if (isStop)
                    {
                        // Update Current Frame with the slider when playback is stopped 
                        currentFrameNo = value;
                        _controlMemImage.GetMemImageData((int)currentFrameNo);
                        RaisePropertyChanged(() => MemImageSource);
                    }
                    else
                    {
                        // Update Current Frame with the slider during playback
                        isStop = true;
                        currentFrameNo = value;
                        _controlMemImage.GetMemImageData((int)currentFrameNo);
                        RaisePropertyChanged(() => MemImageSource);
                        // Continue palyback from Current Frame
                        GetMemImage();
                    }
                }
                catch (PdclibException ex)
                {
                    Messenger.Raise(new InformationMessage("Error: " + ex.Message, "", MessageBoxImage.Error, "Information"));
                }

            }
        }

        // Start Frame of playback 
        public UInt32 StartFrameNo
        {
            get { return startFrameNo; }
            set {
                if (value < _controlMemImage.FrameInfo.m_nStart)
                    startFrameNo = (UInt32)_controlMemImage.FrameInfo.m_nStart;
                else
                    startFrameNo = value;
                // When Start Frame is set to bigger number than Current Frame, Current Frame will also update to the same value
                if (currentFrameNo < startFrameNo) 
                {
                    currentFrameNo = startFrameNo; 
                }

                // Synchronize Start Frame of file saving 
                saveFileStartFrameNo = (int)startFrameNo;
                
                RaisePropertyChanged(() => StartFrameNo);
                RaisePropertyChanged(() => SaveFileStartFrameNo);
                RaisePropertyChanged(() => CurrentFrameNo);
            }
        }

        public UInt32 EndFrameNo
        {
            get { return endFrameNo; }
            set {
                if (value > _controlMemImage.FrameInfo.m_nEnd)
                    endFrameNo = (UInt32)_controlMemImage.FrameInfo.m_nEnd;
                else
                    endFrameNo = value;
                RaisePropertyChanged(() => EndFrameNo);

                // Synchronize End Frame of file saving 
                saveFileEndFrameNo = (int)endFrameNo;
                RaisePropertyChanged(() => SaveFileEndFrameNo);
               
            }
        }

        public UInt32 TotalFrameNo
        {
            get { return _controlMemImage.FrameInfo.m_nRecordedFrames; }
        }

        public BitmapSource MemImageSource
        {
            get { return _controlMemImage.MemImageSource; }
        }

        // Save image file 
        public void SaveImages(SavingFileSelectionMessage fileName)
        {
            if (fileName.Response != null)
            { 
                // Create window of file saving status 
                Window savingFileWindow = new MiniPFV.Views.SavingFileWindow { DataContext = new MiniPFV.ViewModels.SavingFileWindowViewModel(_controlMemImage) };
                _controlMemImage.SaveFile(fileName.Response[0], saveFileStartFrameNo, saveFileEndFrameNo, savingFileWindow);
                savingFileWindow.Show();
            }
                       
        }



        int saveFileStartFrameNo;
        int saveFileEndFrameNo;

        // Start Frame of file saving 
        public int SaveFileStartFrameNo
        {
            get { return saveFileStartFrameNo; }
            set {
                if (value < _controlMemImage.FrameInfo.m_nStart)
                    saveFileStartFrameNo = _controlMemImage.FrameInfo.m_nStart;
                else
                    saveFileStartFrameNo = value;
            }
        }

        // End Frame of file saving 
        public int SaveFileEndFrameNo
        {
            get { return saveFileEndFrameNo; }
            set {
                if (value > _controlMemImage.FrameInfo.m_nEnd)
                    saveFileEndFrameNo = _controlMemImage.FrameInfo.m_nEnd;
                else
                    saveFileEndFrameNo = value;
            }
        }      
    }
}
