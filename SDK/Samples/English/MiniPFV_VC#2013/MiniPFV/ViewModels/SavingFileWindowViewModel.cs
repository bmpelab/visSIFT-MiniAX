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

using System.Windows.Forms;

using MiniPFV.Models;

namespace MiniPFV.ViewModels
{
    public class SavingFileWindowViewModel : ViewModel
    {
        

        public void Initialize()
        {
        }

        IMemoryImageController _controlMemImage;
        public SavingFileWindowViewModel(IMemoryImageController controlMemImage)
        {
            _controlMemImage = controlMemImage;
        }

        // Cancel image file saving 
        public void CancelSaveImages()
        {
            _controlMemImage.CancelSaveFile();
        }
    }
}
