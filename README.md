# device-driver
2021年度ロボットシステム学の課題１で作成したLEDを使ったデバイスドライバです。  
講義の内容通り、1を入力するとLEDが点灯し、0を入力すると消灯します。

# 動作環境
・raspberry pi 4B  
・OS : ubuntu 20.04 server

# 使用した物
・raspberry pi 4B × 1  
・LED × 1  
・抵抗　240Ω × 1  
・ブレッドボード × 1  
・ジャンパー線　(オス-メス)× 2

# 配線
実際の写真![145840682-ddc4927a-470e-43a7-8ca2-4dd4d7a4e4bc](https://user-images.githubusercontent.com/93331260/145842041-31ddea61-1b9a-4a9e-b127-173d34ed0b43.jpg)

# ビルド方法
*インストール*  
    `git clone https://github.com/tamiharu/device-driver.git`  
    
*インストール後の手順*  

    make
    (処理…(途中で警告が入りますが無視で大丈夫です))
    sudo insmod myled.ko
    sudo chmod 666 /dev/myled0

# 実行方法
点灯  
`echo 1 > /dev/myled0`

消灯  
`echo 0 > /dev/myled0`

終了(ドライバをアンインストール)  
`sudo rmmod myled`


# 実行結果
実行した結果を撮影した動画をyoutubeで見る事が出来ます。  
https://www.youtube.com/watch?v=_2moJfYogqU

# ライセンス
GNU General Public License v3.0  
https://github.com/tamiharu/device-driver/blob/main/LICENSE

# その他(著作者等)
著作者  
上田隆一(Ryuichi Ueda)  
伊丹悠人(Haruto Itami)
