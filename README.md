# YouTube-Download

個人使用的 YouTube 下載輔助工具

由 C 語言撰寫，幫助使用者快速下載 YouTube 影片

目前適用於 macOS ，但 Version 3.0 已做好擴展至其他作業系統的準備

需先安裝 Homebrew, yt-dlp, ffmpeg

最後更新：20250207

## 使用說明
**dl \[模式\] \[參數\] \[參數\]...**

### 模式

`u`
軟體更新模式
* 使用 Homebrew 更新 yt-dlp 與 ffmpeg

`a`
音訊下載模式
* 輸入 YouTube 連結，將自動下載最高品質的音訊，並轉檔為 mp3 格式

`v`
影片下載模式
* 輸入 YouTube 連結，將自動下載最高品質的音訊與影片，並合併轉檔為 mov 格式 \(H.264 與 AAC 編碼\)

`t`
縮圖下載模式
* 輸入 YouTube 連結，將自動下載該影片的縮圖

`fa`
讀檔下載模式\(音訊\)
* 輸入檔案名稱，將讀取該檔案，依序下載每一個 YouTube 音訊。檔案每行包含一個 YouTube 連結
  * 若未輸入檔案名稱，預設讀取 `url.txt`

`fv`
讀檔下載模式\(影片\)
* 輸入檔案名稱，將讀取該檔案，依序下載每一個 YouTube 影片。檔案每行包含一個 YouTube 連結
  * 若未輸入檔案名稱，預設讀取 `url.txt`

`ft`
讀檔下載模式\(縮圖\)
* 輸入檔案名稱，將讀取該檔案，依序下載每一個 YouTube 縮圖。檔案每行包含一個 YouTube 連結
  * 若未輸入檔案名稱，預設讀取 `url.txt`

### 參數

`-k`
在轉檔後保留原始下載檔案
* 在 `a`, `v`, `fa`, `fv` 模式中可用

`-n`
不執行 ffmpeg 轉檔
* 在 `a`, `v`, `fa`, `fv` 模式中可用

`-c`
自訂下載的檔案名稱
* 在 `a`, `v`, `t`, `fa`, `fv`, `ft` 模式中可用
  * 在 `fa`, `fv`, `ft` 模式中，若未輸入檔案名稱，預設讀取 `name.txt`

`-i`
在檔案名稱中加入索引值
* 在 `fa`, `fv`, `ft` 模式中可用

`-m4a`
下載音訊時轉檔為 m4a 格式，而非 mp3
* 在 `a`, `fa` 模式中可用

`-p`
輸出執行的指令
* 在所有模式中可用

### 範例指令

`dl a`
此指令會下載給定的 YouTube 音訊

`dl v -k`
此指令會下載給定的 YouTube 影片，並在轉檔後保留原始下載檔案

`dl a -m4a`
此指令會下載給定的 YouTube 音訊，並轉檔為 m4a 格式
