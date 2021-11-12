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
        /// カメラヘッド名の取得
        /// </summary>
        string Name { get; }

        /// <summary>
        /// デバイスナンバーの取得
        /// </summary>
        UInt32 DeviceNo { get; }

        /// <summary>
        /// 子デバイスナンバーの取得
        /// </summary>
        UInt32 ChildNo { get; }

        /// <summary>
        /// カラータイプの取得
        /// </summary>
        ColorType ColorType { get; }

        /// <summary>
        /// デバイス名の取得
        /// </summary>
        string DeviceName { get; }

        /// <summary>
        /// エラーメッセージの取得
        /// </summary>
        string ErrorMessage { get; }
    }
}
