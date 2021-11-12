using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MiniPFV.Models
{
    /// <summary>
    /// カメラコントロール用インターフェース
    /// </summary>
    public interface IControlCamera
    {
        /// <summary>
        /// エラーメッセージの取得
        /// </summary>
        string ErrorMessage { get; }
        
        /// <summary>
        /// カメラ初期化
        /// </summary>
        /// <returns>true:成功、false:失敗</returns>
        bool Init();

        /// <summary>
        /// カメラヘッドコントロール用インターフェースの取得
        /// </summary>
        IList<IControlCameraHead> CameraHeads { get; } 

    }
}
