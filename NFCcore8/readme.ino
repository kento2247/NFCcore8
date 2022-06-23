/*
 * 
 * 接続(ESP32,Arduinoの場合)
 * -RST : setup(byte SDA, byte RST, bool SelectDumpInfo) の引数で指定したピン
 * -SCK : ボード固定のSPI通信用CLKピン、ピンアサイン参照
 * -SDA : setup(byte SDA, byte RST, bool SelectDumpInfo) の引数で指定したピン。CSチップセレクトピンとも言う
 * -MISO : ボード固定のSPI通信用MISOピン、ピンアサイン参照
 * -MOSI : ボード固定のSPI通信用MOSIピン、ピンアサイン参照
 * 
 * NFCcore8はNFCcore6で取り入れたKHEC-KCG共通規格(https://docs.google.com/spreadsheets/d/19-Z5d7bw4iWchVB4cAa4Gvx5h9HcvqutWEYCGTYfdFQ/edit?usp=sharing)
 * とNFCcore7bで採用したclass構造をマージしたものです。基本的にreadしたデータはclass内public領域にある配列に記録されます。またwriteをするときに書き込むデータは、write関数
 * 呼び出し前に、同じくpublic領域にある配列に記録しておく必要があります。
 * 
 * ver8の大きな変更点
 * １、Date[]オブジェクトをclass内のpublicに用意しました。入退室管理などを行う場合は、このDateオブジェクト内に最新の時刻を記録することで、自動的にカード内に最終記録時刻が保存される仕組みとなります。
 * ２、共通規格に準拠し、ブロック1,2内に保存されている以下の情報を管理できるようになります
 * 　　（規格バージョン、最終記録時刻、ユーザー名）
 * 
 * 
 * 追加予定昨日
 * １、authをする際に、キーを任意のものを引数で渡せるようにする。
 * ２、1を実装するためにも、汎用性を重視したWrite/Read関数を作成。デフォルトキーか任意キーかを選べるように
 * ３、ブロック1に保存されている団体番号、部員番号の使用開始。
 * ４、現在の設定情報を一覧表示する関数を作成。
 * ５、シリアル通信スピードなど、各種詳細設定も可能にする。また複雑化回避のため、デフォルト値を設けることとする。
 * ６、エラーコード<->エラーメッセージ関数を作成する。
 * 
 * 
 * 
 */
