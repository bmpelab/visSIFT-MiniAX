using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MiniPFV.Models
{
    /// <summary>
    /// Interface for camera control
    /// </summary>
    public interface IControlCamera
    {
        /// <summary>
        /// Get error message
        /// </summary>
        string ErrorMessage { get; }
        
        /// <summary>
        /// Initialize camera
        /// </summary>
        /// <returns>true:success、false:fail</returns>
        bool Init();

        /// <summary>
        /// Get interface for camera head controlling 
        /// </summary>
        IList<IControlCameraHead> CameraHeads { get; } 

    }
}
