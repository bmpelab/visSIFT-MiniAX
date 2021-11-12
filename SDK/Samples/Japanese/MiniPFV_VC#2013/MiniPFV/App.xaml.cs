using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Windows;

using Livet;
using MiniPFV.Models;

namespace MiniPFV
{
    /// <summary>
    /// App.xaml の相互作用ロジック
    /// </summary>
    public partial class App : Application
    {
        private void Application_Startup(object sender, StartupEventArgs e)
        {
            DispatcherHelper.UIDispatcher = Dispatcher;
            AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler(CurrentDomain_UnhandledException);
            
            // シャットダウンモードを指定。（こちらが指定するまでシャットダウンしないようにする）
            Application.Current.ShutdownMode = System.Windows.ShutdownMode.OnExplicitShutdown;

            // 初期化ウィンドウの表示
            var openCloseCamera = new MiniPFV.Models.OpenCloseCamera();
            var iniWindow = new MiniPFV.Views.InitializeWindow() { DataContext = new MiniPFV.ViewModels.InitializeWindowViewModel(openCloseCamera) };
            iniWindow.ShowDialog();

            // カメラが見つかっている場合ウィンドウの表示
            if (openCloseCamera.Camera != null )
            { 
                var mainWindowViewModel = new MiniPFV.ViewModels.MainWindowViewModel(openCloseCamera.Camera);
                var mainWindow = new MiniPFV.Views.MainWindow() { DataContext =  mainWindowViewModel};
                    mainWindow.ShowDialog();
                mainWindowViewModel.IsWindowClosed = true;
            }
            
            // カメラを閉じる
            openCloseCamera.Close();

            // アプリケーションを閉じる            
            this.Shutdown();
        }

        // 集約エラーハンドラ
        private void CurrentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
            //TODO:ロギング処理など
            MessageBox.Show(
                "不明なエラーが発生しました。アプリケーションを終了します。",
                "エラー",
                MessageBoxButton.OK,
                MessageBoxImage.Error);

            Environment.Exit(1);
        }

    }
}
