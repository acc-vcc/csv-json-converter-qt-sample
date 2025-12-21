# CSV ⇄ JSON Converter (Qt Sample)

このプロジェクトは、Qt を使用した CSV ⇄ JSON 変換ツールのサンプルです。
CSV の読み込み、ヘッダ行の自動判定、JSON 形式への変換、および逆変換を行います。
副業・業務委託向けのサンプルコードとして作成しています。

## Features

- GUI アプリケーション
- CSV → JSON 変換
- JSON → CSV 変換
- 入力ファイルが CSV / JSON のどちらかを自動判定
- ヘッダ行の自動判定（1 行目と 2 行目の内容を比較）
- 改行やクォートを含む CSV に対応
- Qt（QFile / QTextStream / QJsonDocument）を使用した実装例

## Build

1. Qt Creator でプロジェクトを開く
2. Kit を選択する
3. Build & Run を実行

## Usage

### CSV → JSON

1. 「入力ファイル選択」から CSV ファイルを選択
2. 「変換実行」をクリック
3. JSON ファイルが出力されます

### JSON → CSV

1. 「入力ファイル選択」から JSON ファイルを選択
2. 「変換実行」をクリック
3. CSV ファイルが出力されます

## Directory Structure

```
csv-json-converter-qt-sample/
├─ .gitignore             # Git 管理対象外ファイル設定
├─ .github
│  └─ workflows
│     └─ build.yml        # 自動ビルド用 GitHub Actions ファイル
├─ main.cpp               # エントリポイント
├─ mainwindow.cpp         # メインウィンドウの実装
├─ mainwindow.h
├─ mainwindow.ui          # UI レイアウト
├─ ConvCsvJson.pro        # Qt プロジェクトファイル
├─ CMakeLists.txt         # CMake 用設定ファイル
├─ Dockerfile             # 環境構築用 Dockerfile
└─ README.md              # プロジェクト説明
```

## CI/CD (GitHub Actionsによる自動ビルド)
本プロジェクトには、GitHub Actions を用いた CI/CD を導入しています。
Qt GUI アプリのビルド環境を Docker イメージとして管理し、
**再現性の高いビルド**と**自動アーティファクト生成**を行います。

### 主なポイント
- Dockerfile の変更検知による環境イメージ更新  
  Git の差分を用いて Dockerfile の変更を検出し、
  **Dockerfile に変更があった場合のみ** GHCR の環境イメージを再ビルド・再 push します。
  変更がない場合は既存のイメージをそのまま利用します。

- 毎回クリーンな Qt ビルド  
  Docker コンテナ内で以下を実行します：

```
cmake ..
make -j$(nproc)
```

- ビルド成果物の自動アップロード  
  成果物（ConvCsvJson 実行ファイル）は GitHub Actions の artifact として保存されます。

## Important Notes

- JSON の仕様上、オブジェクトのキー順は保証されません。
- Qt の `QJsonObject` はキーをアルファベット順に並べ替えるため、  
  CSV → JSON → CSV の往復で列順が変わる場合があります。
- サンプルコードのため、列順保持などの高度な仕様は実装していません。
- 改行コードの保持は行いません。
- 拡張子が `.csv` でない場合、またはファイル先頭 20 文字以内にカンマが存在しない場合、CSV と判定できません。

## .gitignore

Qt Creator のユーザ設定ファイル（`*.pro.user`）やビルド生成物は
Git に含めないよう `.gitignore` を設定しています。

## License

MIT License

