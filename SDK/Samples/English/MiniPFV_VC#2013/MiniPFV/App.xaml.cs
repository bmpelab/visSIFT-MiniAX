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
    /// Interaction logic of App.xaml
    /// </summary>
    public partial class App : Application
    {
        private void Application_Startup(object sender, StartupEventArgs e)
        {
            DispatcherHelper.UIDispatcher = Dispatcher;
            AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler(CurrentDomain_UnhandledException);
            
            // Designate shutdown mode (Do not shutdown until the designation is finished)
            Application.Current.ShutdownMode = System.Windows.ShutdownMode.OnExplicitShutdown;

            // Show Initialize Window 
            var openCloseCamera = new MiniPFV.Models.OpenCloseCamera();
            var iniWindow = new MiniPFV.Views.InitializeWindow() { DataContext = new MiniPFV.ViewModels.InitializeWindowViewModel(openCloseCamera) };
            iniWindow.ShowDialog();

            // Behavior when camera is found
            if (openCloseCamera.Camera != null )
            { 
                var mainWindowViewModel = new MiniPFV.ViewModels.MainWindowViewModel(openCloseCamera.Camera);
                var mainWindow = new MiniPFV.Views.MainWindow() { DataContext =  mainWindowViewModel};
                    mainWindow.ShowDialog();
                mainWindowViewModel.IsWindowClosed = true;
            }
            
            // Close camera
            openCloseCamera.Close();

            // Finish the application        
            this.Shutdown();
        }

        // Intensive error handler 
        private void CurrentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
            //TODO: logging process etc.
            MessageBox.Show(
                "Unknown error occured. The application will be finished.",
                "Error",
                MessageBoxButton.OK,
                MessageBoxImage.Error);

            Environment.Exit(1);
        }

    }
}
