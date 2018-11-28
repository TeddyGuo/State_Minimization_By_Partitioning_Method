# State_Minimization_By_Partitioning_Method

測試環境為	CentOS
使用的編譯器為 	g++

編譯指令：
g++ state_mini.cpp -o *

P.S. '*' 為萬用字符

執行方法：
./* INPUT.kiss OUTPUT.kiss OUTPUT.dot

如果想視覺化 dot file 可以在 CentOS 環境安裝 graphviz

安裝指令：
sudo yum install graphviz

執行方法：
dot -T png OUTPUT.dot > OUTPUT.png
