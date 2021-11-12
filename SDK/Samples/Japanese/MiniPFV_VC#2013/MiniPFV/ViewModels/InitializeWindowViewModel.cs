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
    // 初期化の状態
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
        /* コマンド、プロパティの定義にはそれぞれ 
         * 
         *  lvcom   : ViewModelCommand
         *  lvcomn  : ViewModelCommand(CanExecute無)
         *  llcom   : ListenerCommand(パラメータ有のコマンド)
         *  llcomn  : ListenerCommand(パラメータ有のコマンド・CanExecute無)
         *  lprop   : 変更通知プロパティ(.NET4.5ではlpropn)
         *  
         * を使用してください。
         * 
         * Modelが十分にリッチであるならコマンドにこだわる必要はありません。
         * View側のコードビハインドを使用しないMVVMパターンの実装を行う場合でも、ViewModelにメソッドを定義し、
         * LivetCallMethodActionなどから直接メソッドを呼び出してください。
         * 
         * ViewModelのコマンドを呼び出せるLivetのすべてのビヘイビア・トリガー・アクションは
         * 同様に直接ViewModelのメソッドを呼び出し可能です。
         */

        /* ViewModelからViewを操作したい場合は、View側のコードビハインド無で処理を行いたい場合は
         * Messengerプロパティからメッセージ(各種InteractionMessage)を発信する事を検討してください。
         */

        /* Modelからの変更通知などの各種イベントを受け取る場合は、PropertyChangedEventListenerや
         * CollectionChangedEventListenerを使うと便利です。各種ListenerはViewModelに定義されている
         * CompositeDisposableプロパティ(LivetCompositeDisposable型)に格納しておく事でイベント解放を容易に行えます。
         * 
         * ReactiveExtensionsなどを併用する場合は、ReactiveExtensionsのCompositeDisposableを
         * ViewModelのCompositeDisposableプロパティに格納しておくのを推奨します。
         * 
         * LivetのWindowテンプレートではViewのウィンドウが閉じる際にDataContextDisposeActionが動作するようになっており、
         * ViewModelのDisposeが呼ばれCompositeDisposableプロパティに格納されたすべてのIDisposable型のインスタンスが解放されます。
         * 
         * ViewModelを使いまわしたい時などは、ViewからDataContextDisposeActionを取り除くか、発動のタイミングをずらす事で対応可能です。
         */

        /* UIDispatcherを操作する場合は、DispatcherHelperのメソッドを操作してください。
         * UIDispatcher自体はApp.xaml.csでインスタンスを確保してあります。
         * 
         * LivetのViewModelではプロパティ変更通知(RaisePropertyChanged)やDispatcherCollectionを使ったコレクション変更通知は
         * 自動的にUIDispatcher上での通知に変換されます。変更通知に際してUIDispatcherを操作する必要はありません。
         */

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
                // デバイスを検索する
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
                    // 検索完了したら、ウインドを閉じる
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

        // IPアドレス初期値
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
