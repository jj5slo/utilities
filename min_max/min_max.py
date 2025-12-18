import pandas as pd
import glob
import os

# 設定
target_dir = './data_directory'  # .datファイルがあるディレクトリ
output_file = 'min_max.txt'       # 出力ファイル名
file_pattern = os.path.join(target_dir, '*.dat')

# 1. 対象ファイルをすべて取得
files = glob.glob(file_pattern)

if not files:
    print("ファイルが見つかりませんでした。")
    exit()

# 2. 全ファイルを読み込んでリストに格納
df_list = []
for file in files:
    # sep='\s+' はスペースまたはタブ区切りを意味します
    # index_col=0 は1列目(index)を行ラベルとして扱う設定です
    # header=None はファイルに列名行がない場合です（ある場合は header=0 に変更）
    try:
        df = pd.read_csv(file, sep='\s+', header=None, index_col=0)
        df_list.append(df)
    except Exception as e:
        print(f"Error reading {file}: {e}")

# 3. 全データを結合
# 全てのデータフレームを縦に結合します（indexが同じものはグループ化の準備になります）
combined_df = pd.concat(df_list)

# 4. indexごとにグループ化し、各列のminとmaxを計算
# agg(['min', 'max']) で各列に対して最小値と最大値を計算します
result = combined_df.groupby(level=0).agg(['min', 'max'])

# 5. 列名の整形 (例: 1_min, 1_max, 2_min, 2_max ...)
# 現在の列名は (列番号, 'min') のような階層構造になっているため、平坦化します
new_columns = []
for col_idx, stat in result.columns:
    # 元のデータ列名が数値（1, 2...）の場合と文字列（data1...）の場合で調整
    new_columns.append(f"data{col_idx}_{stat}")

result.columns = new_columns

# 6. 結果をファイルに出力
# indexも含めてスペース区切りで出力
result.to_csv(output_file, sep=' ')

print(f"処理が完了しました。'{output_file}' を確認してください。")
print("--- 先頭5行のプレビュー ---")
print(result.head())
