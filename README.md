# visSIFT-MiniAX
## About
Photron Mini AX で Structure from Motion の input 画像を撮影するために，<br>
オンラインで特徴量マッチングを可視化するソフトウェアです．
  
環境は以下の通り．  
- Windows 10 Pro (Version 1909)
- Microsoft Visual Studio Community 2019 (Version 16.7.3)

## セットアップ
### Visual Studio2019のインストール
以下のサイトからインストール．<br>
ECCSアカウントでダウンロードできる．<br>
https://visualstudio.microsoft.com/ja/downloads/

### Photron提供のSDKからライブラリを追加
~/visSIFT-MiniAX/PhotronAPI.sln を Visual Studio 2019 で開く．<br>
以下のサイトから Photron Software Package 4.0.3.0 をダウンロードして分かりやすい場所に置いておく．<br>
https://photron.com/software-downloads/ <br>
もしくは Mini AX 付属の DVD から，PFV4_DVDImage(V4.0.3.0) フォルダをダウンロードしてきてもよい．<br>
<br>
ライブラリ追加の方法は (パッケージ最上位フォルダ)/SDK/Doc/PDCLIB_JP に書いてあるが，同じ内容を以下にも記述する．<br>
Visual Studioのメニューの プロジェクト->プロパティ から以下のように文字列を追加．<br>
- リンカー -> 全般 -> 追加のライブラリディレクトリ
PDCLIB.libが入ってるフォルダへのフルパスを追加．((パッケージ最上位フォルダ)/SDK/Lib/64bit(x64))
- リンカー -> 入力 -> 追加の依存ファイル
　「PDCLIB.lib」を追加．
- C/C++ -> 全般 -> 追加のインクルードディレクトリ
　追加のインクルード ディレクトリにincludeディレクトリのフルパスを追加．((パッケージ最上位フォルダ)/Include)

### dllフォルダへのパスを環境変数に追加
プログラムを実行するために，dllフォルダ内の各dllファイルにパスを通す必要がある．<br>
コントロールパネル → システムとセキュリティ → システム → システムの詳細設定 → 環境変数 → 「Path」を編集<br>
以下の文字列を追加↓<br>
(前略)\visSIFT-MiniAX\dll<br>
PCを再起動<br>

### OpenCV のインストール
OpenCV 4.4.0 以前では，SIFTはOpenCV contribに入っていたため，Cmakeを用いてソースコードから2つを一緒にビルドする必要があった．<br>
（ググったらまだこの方法が沢山出てくると思う．）<br>
しかし，OpenCV 4.4.0 ではSIFTの特許が切れてメインストリームに移管されたため，単純に<br>
https://opencv.org/releases/ <br>
から OpenCV 4.4.0 のWindowsインストーラーをダウンロードして実行し，インストールすればOK．<br>
分かりやすい場所にExtractする．

### Visual StudioでOpenCVのパスを通す
Visual Studioのメニューの「プロジェクト→Photron API」のプロジェクトから以下のように文字列を追加．<br>
- リンカー -> 全般 -> 追加のライブラリディレクトリ
(openCVのインストール先)\opencv-4.4.0\build\x64\vc15\lib を追加
- リンカー -> 入力 -> 追加のライブラリディレクトリ
　「opencv_world440d.lib」と「opencv_world440.lib」を追加
- C/C++ -> 全般 > 追加のインクルードディレクトリ
(openCVのインストール先)\opencv-4.4.0\build\include を追加 <br>
環境変数のPathにOpenCVの dll が格納されている<br>
(openCVのインストール先)\opencv-4.4.0\install\x64\vc15\bin を追加．<br>
PCを再起動．<br>
<br>
以上の設定により，C++のプログラム上で<br>
#include <opencv2/opencv.hpp> <br>
using namespace cv; <br>
と記述すればOpenCVを使える．

### ファイヤーウォールの設定
以上の設定を行ったうえでプログラムを実行した際に，カメラの検出が行えない場合が存在する．<br>
ファイヤーウォールに遮断されているのが原因だった場合，以下の設定で直せる．<br>
コントロールパネル → システムとセキュリティ → Windows Defender ファイヤーウォール → Windows Difender ファイヤーウォールを介したアプリまたは機能を許可<br>
から，「別のアプリの許可」で，PhotronのSDKの実行ファイルに許可設定を与える．<br>
C:\Users\furuk\source\PhotronAPI\x64\Release\PhotronAPI.exe <br>

### ネットワーク設定の変更
インターネットの接続とPhotron機種の接続設定が両立できないため，Photron機種用にネットワーク設定を変更する．<br>
カメラとPCをイーサネットケーブルで繋げた状態でPFV4を立ち上げて「詳細設定→PCのネットワーク設定」から「カメラ通信用にIPアドレスを設定する」で「192.168.0.1」（最後は10じゃなければ何でもOK）に変更する．<br>
PFV4を再起動．<br>
<br>
なお，逆にインターネットを使えるようにするためには，イーサネットを付け替えて<br>
Windowsメニュー→ネットワークとインターネット→接続プロパティの変更→IP設定→編集→「自動（DHCP）」を選択して「保存」<br>
で直せる．

## プログラムの実行
PhotronAPI.sln <br>
を開いて実行．
