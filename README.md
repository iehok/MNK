# 生得分離モデルに基づく多目的遺伝的アルゴリズムの大域的探索性能の評価

## 概要
生得分離モデルの再帰的な初期化戦略にDIRECTの再帰的な空間分割戦略を適用した新しい探索手法を提案する。
これは、多目的最適化問題に対し、並列にGAを行うフレームワークである。
加えて、多目的NKモデルのベンチマーク問題を新たに作成し、提案手法の大域的探索性能を評価した。
実験の結果より、提案手法は、設計変数空間上での大域的探索性能の観点で、ベースラインを上回ることがわかった。

## パラメータの設定
染色体長や目的数といった重要なパラメータは`config.h`に記載されています。
これらのパラメータは、GAによる探索や真のパレート最適解の探索だけでなく、探索結果の分析の際にも用いられます。

## GAによる探索

### 探索の実行
```
g++ -O2 execute_ga.cpp
```
```
./a.out
```
`checkpoint`フォルダ内に各試行ごとの探索結果が出力されます。

### 探索結果のマージ
```
g++ -O2 merge_results.cpp
```
```
./a.out 128
```
実行時引数は、試行数以上となる2のべき乗にしてください。
例えば、試行数が100の場合、実行時引数は128となります。
正しく処理されると、`checkpoint`フォルダ内に新しく、マージの結果である`checkpoint.csv`というファイルが出力されます。

## 真のパレート最適解の探索
### 準備
まず、`search_truePareto.cpp`と`merge_truePareto.cpp`内の変数`OFFSET_LEN`を編集します。
`OFFSET_LEN`は探索の間中固定しておくビットの長さを表しており、固定していないビットを全探索します。
例えば、`config.h`中の`N`が40、`search_truePareto.cpp`中の`OFFSET_LEN`が8の場合、40ビットのうち8ビットを固定し、残りの32ビットを全探索（2^32個の染色体を評価）します。

### 探索の実行
```
bash job.sh
```
実行すると、`offset_value_lower`，`offset_value_upper`，`max_jobn`を入力するように言われます。
`offset_value_lower`と`offset_value_upper`は探索するオフセットの10進数の値の区間を表しています。
例えば、`search_truePareto.cpp`中の`OFFSET_LEN`が8の場合、`offset_value_lower`は0から2^8-1(255)までの値になります。`offset_value_upper`は、とりあえず最も大きい値（今回であれば255）としてよいかもしれません。
`true_pareto`フォルダ内に各オフセットの値ごとの探索結果が出力されます。

### 探索結果のマージ
```
g++ merge_truePareto.cpp
```
```
./a.out
```
正しく処理されると、`true_pareto`フォルダ内に新しく、マージの結果である`true_pareto.csv`というファイルが出力されます。

## 連絡先
質問やご意見等ございましたら、小田康平（s2420017@jaist.ac.jp）までご連絡ください。
