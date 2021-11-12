using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MiniPFV.Models
{
    /// <summary>
    /// Result of camera initialization 
    /// </summary>
    public enum Result
    {
        Completed,
        Canceled,
        Error,
    };

    /// <summary>
    /// Interface for camera detecting and close
    /// </summary>
    public interface IOpenCloseCamera
    {
        /// <summary>
        /// Detect camera
        /// </summary>
        /// <param name="IPAdress">IP address（fixed）</param>
        /// <param name="message">message when camera detecting failed</param>
        /// <returns></returns>
        Result DetectCamera(UInt32 IPAdress, out string message);
        
        /// <summary>
        /// Abort camera detecting
        /// </summary>
        void Abort();

        /// <summary>
        /// Close camera
        /// </summary>
        void Close();

        /// <summary>
        /// Interface for camera controlling
        /// </summary>
        IControlCamera Camera { get; }

    }
}
