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
using System.Threading;

namespace MiniPFV.ViewModels
{
    // Status of Initialization
    public enum InitializeStatus
    {
        Start,
        Detecting,
        Waiting,
        Completed,
        Canceled,
        Error,
    };

    public class InitializeWindowViewModel : ViewModel
    {
        

        private IOpenCloseCamera _openClose;
        public void Initialize()
        {
        }

        public InitializeWindowViewModel(IOpenCloseCamera openClose)
        {
            _openClose = openClose;
        }

        public void Start()
        {
            status = InitializeStatus.Detecting;
            RaisePropertyChanged(() => Status);

            errorMessage = string.Empty;
            RaisePropertyChanged(() => ErrorMessage);

            UInt32 address = (UInt32)((ipAddress0 << 24) + (ipAddress1 << 16) + (ipAddress2 << 8) + ipAddress3); 
            var t = Task.Factory.StartNew(() => 
            {
                // Detect device
                Result result = _openClose.DetectCamera(address, out errorMessage);
                switch (result)
                {
                    case Result.Completed:
                        status = InitializeStatus.Completed;
                        break;
                    case Result.Canceled:
                        status = InitializeStatus.Canceled;
                        break;
                    case Result.Error:
                        RaisePropertyChanged(() => ErrorMessage);
                        status = InitializeStatus.Error;
                        break;
                    default:
                        break;
                }
                RaisePropertyChanged(() => Status);
                if (result == Result.Completed)
                {   
                    // Close the window when detecting is finished
                    Messenger.Raise(new WindowActionMessage(WindowAction.Close, "Complete"));
                }
            });

        }

        public void Cancel()
        {
            status = InitializeStatus.Waiting;
            RaisePropertyChanged(() => Status);
            _openClose.Abort();
        }

        string errorMessage;
        public string ErrorMessage
        {
            get { return errorMessage; }
        }

        InitializeStatus status = InitializeStatus.Start;
        public InitializeStatus Status
        {
            get { return status; }
        }

        // Initial value of IP address
        int ipAddress0 = 192;
        int ipAddress1 = 168;
        int ipAddress2 = 0;
        int ipAddress3 = 10;
        public int IPAddress0
        {
            get { return ipAddress0; }
            set { ipAddress0 = CheckIPAddress(value); }
        }
        public int IPAddress1
        {
            get { return ipAddress1; }
            set { ipAddress1 = CheckIPAddress(value); }
        }
        public int IPAddress2
        {
            get { return ipAddress2; }
            set { ipAddress2 = CheckIPAddress(value); }
        }
        public int IPAddress3
        {
            get { return ipAddress3; }
            set { ipAddress3 = CheckIPAddress(value); }
        }

        private int CheckIPAddress(int ip)
        {
            if (ip < 0)
                return 0;
            if (ip > 255)
                return 255;
            else
                return ip;
        }
    }
}
