using MiniPFV.ViewModels;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MiniPFV.Models
{
    public interface IControlCameraHead
    {
        /// <summary>
        /// Get name of camera head
        /// </summary>
        string Name { get; }

        /// <summary>
        /// Get device number
        /// </summary>
        UInt32 DeviceNo { get; }

        /// <summary>
        /// Get child device number
        /// </summary>
        UInt32 ChildNo { get; }

        /// <summary>
        /// Get color type of camera
        /// </summary>
        ColorType ColorType { get; }

        /// <summary>
        /// Get name of device
        /// </summary>
        string DeviceName { get; }

        /// <summary>
        /// Get error message
        /// </summary>
        string ErrorMessage { get; }
    }
}
