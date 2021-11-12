using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MiniPFV.Models
{
    /// <summary>
    /// カメラ初期化の結果
    /// </summary>
    public enum Result
    {
        Completed,
        Canceled,
        Error,
    };

    /// <summary>
    /// カメラの検索、終了処理用インターフェース
    /// </summary>
    public interface IOpenCloseCamera
    {
        /// <summary>
        /// カメラ検索
        /// </summary>
        /// <param name="IPAdress">IPアドレス（固定）</param>
        /// <param name="message">失敗時のメッセージ</param>
        /// <returns></returns>
        Result DetectCamera(UInt32 IPAdress, out string message);
        
        /// <summary>
        /// 途中中断
        /// </summary>
        void Abort();

        /// <summary>
        /// カメラ終了処理
        /// </summary>
        void Close();

        /// <summary>
        /// カメラコントロール用インターフェースの取得
        /// </summary>
        IControlCamera Camera { get; }

    }
}
