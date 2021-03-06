【ソフトウェア名】アナログ時計
【バージョン】 1.0
【製作者】松澤 大地
【連絡先】メール:17429@g.nagano-nct.ac.jp


【概要】
	本ソフトウェアはアナログ時計を表示するソフトウェアです。現在時刻を時針(短針)、分針(長針)、秒針(長針と同じ長さ、色は赤)で表示します。
	背景色を白くするライトモードおよび背景色を黒くするダークモードが利用できます。
	実行時の時間を取得し、昼(7～18時)であればライトモード、夜(19～6時)であればダークモードで表示します。なお、背景色は切替可能です。
	また、アナログ時計以外にも他の時計を表示できる時計モードの切替も可能です。
	現在時刻と現在の年、月、日、曜日を文字列として表示するデジタル時計モード、7セグメントLED風の時計を表示する7セグメントLED時計モードが選択できます。

【内容物】
	Readme.txt	# 本ファイル
	j17429.c	# Cソースファイル
	myicon.ico	# アイコンファイル
	myicon.rc	# アイコンからオブジェクトファイルを作成するファイル
	myicon.o	# アイコンを実行ファイルに紐付けるオブジェクトファイル
	Makefile	# ビルド時に用いるファイル

【動作環境】
	・Windows10 64bit版
	・Cygwin 32bit版
	
【ビルド方法】
	本ファイルが入っているディレクトリ(j17429)内で「make」とコマンドを打ち、プログラムをビルドしてください。
	なお、‘glutCreateMenu_ATEXIT_HACK’ defined but not usedという警告が出る場合がありますが、プログラムの動作には問題ありません。
	
【アンインストール方法】
	本ファイルが入っているディレクトリを削除すればアンインストールは完了です。

【実行方法】
	実行方法は、Cygwinのコマンドライン上で本ファイルが入っているディレクトリに移動し
	$ ./j17429.exe
  	とコマンドを入力して実行します。
	もしくは、エクスプローラーなどから、本ファイルが入っているディレクトリに移動し、j17429.exeのファイルをダブルクリックすることでも実行可能です。
	その際、「cygwin.dllが見つかりません。」というエラーが出る方は、環境変数のPathを「C:cygwin/bin」に通してください。
	また、「glut32.dllが見つからないため、コードの実行を続行できません。」といエラーが出る方は、GLUTまたはglpngのインストールができていない可能性があります。
	次に示すwebページからインストールを行ってください。
	http://teacher.nagano-nct.ac.jp/ito/Springs_of_C/	
	
【使い方】
	実行するとアナログ時計が表示されます。背景色は実行を行った時間帯で変化します。
	
	・背景色の変更
	背景色を変更したい場合は、キーボードのWキーを押すとライトモードに、Bキーを押すとダークモードに変更されます。
	変更後はコンソールにライトモードなら「White」ダークモードなら「Black」と表示されます。
	また、背景色の変更は時計の種類に関わらず変更可能です。
	
	・時計の種類の変更
	時計の種類を変更したい場合は、Dキーを押すとデジタル時計に、Sキーを押すと7セグメントLED時計に変更されます。なお、Aキーを押すとアナログ時計に戻すことができます。
	変更後はコンソールにアナログ時計なら「Analog」デジタル時計なら「Digital」7セグメントLED時計なら「SevenSegment」と表示されます。

	・ウィンドウのサイズ変更
	ウィンドウのサイズを変更すると、それに応じて時計の大きさが変更されます。
	ただし、デジタル時計ではウィンドウサイズは400x400ピクセルで固定しており、変更しても自動で戻ります。
	
	・終了
	プログラムを終了する際は、ウィンドウの閉じるボタンをクリックするかEscキーまたはQキーを押してください。キーボードから終了を行うとコンソールに「Quit」と表示されます。


【注意事項】
	インストールフォルダ内のファイルは依存関係にあるため、削除すると正しい動作をしない可能性があります。

【免責】
	本ソフトウェアを利用して発生したいかなる事態に対しても製作者は一切の責任を負いません。


【バージョン履歴】
	2020.11.02　0.0 作成開始
	2020.11.19  0.1 アナログ時計に目盛り追加
	2020.11.21  0.2 ライトモードおよびダークモードの実装
	2020.11.22  0.3 7セグメントLED時計の追加
	2020.11.24  1.0 初版

以上