# Lexical_Analysis
這次的程式實現了Lexical Analyzer的功能，能夠將原始的程式碼轉換成Token。

# 開發環境與平台：
Window10, Dev C++
# 程式語言：
C++
# 選擇的組語：S
IC
# 程式設計：
# (a)	說明：
    這次的程式實現了Lexical Analyzer的功能，能夠將原始的程式碼轉換成Token。
# (b)	流程： 
    讀入使用者指令之後，會先叫使用者把Table1~4的檔名(包含副檔名)輸入進我們的程式中，程式就會去存取這四組Table，接著就會開始read File。在read File的過程中，我們會先使用一個while迴圈讀入所有的input，此處的條件式就設為「當讀ch不為空時(ch是一個char的字元)」，這樣我們就能在這個迴圈內讀入所有的input。在迴圈內，首先我們會先判斷這個ch是不是delimiter：如果是的話，它就會判斷我們愈來記錄的token裡面有沒有裝東西，如果有裝東西的話，就代表我們遇到一個token後面接一個delimiter，那我們就會看看這個ch是不是等於”\’”，如果是的話，就先記錄起ch，還要考慮那個token的開頭是不是C或X，如果是的話就要繼續讀讀到出現下一個ch為止。如果不是的話紀錄 token在記錄ch即可。那如果前一個token為空的話，紀錄ch即可。那如果ch不是delimiter的話，我們就再判斷它是不是空格，如果是的話就設定token座標並記錄起來。那如果ch不是上述說的任何一項的話，就讓這個ch加進token後面即可。那如果ch是換行的話，那就要記錄此行。以上就是所有while迴圈的內容。那在最後while迴圈後面，會再判斷我們這個token是不是空的，如果不是的話就紀錄token即可。最後在記錄此line，read File就到此為止，只要印出結果即可完成。
# (c)	使用的data structure：
    class Table：還有分成Default Table以及Hash Table，Default Table就是用來裝最一開始讀入的Table1~4，hash Table就是用來裝symbol，literal，以及string。class Token Coordinate：用來存放那個token的座標，裡面會有table Index以及index。class Analyzer：用來讀取input、並且分析token的。裡頭會有用來存table的兩個vector名為table和d table，用來存input content的vector名為input，以及用來存token座標的vector名為coordinates。


# 使用方式：
    請先將Input和Table1~4都放置跟檔案一樣的位置，然後輸入你要的模式(一般使用者使用Regular即可，Debug Mode是我寫給開發者用的)。樹入完之後，程式會要求你輸入你的table檔名，輸入完成再輸入input檔名，程式就會把結果全都印在SIC_output之中。

