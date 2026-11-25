# コーディングスタイルガイド（SideScrollShmup 準拠）

このドキュメントは、本プロジェクト（DxLib + C++20 の横スクロールシューティング）のソースコードを
お手本として、そのコードの書き方・設計パターンをまとめたものです。
新しいクラスや機能を追加するときは、ここに書かれたルールに合わせてください。

---

## 1. 開発環境・前提

| 項目 | 内容 |
|---|---|
| 言語 | C++20（`stdcpp20`）。`std::map::contains` / `std::erase_if` などを使用 |
| ライブラリ | DxLib |
| 文字セット | Unicode（文字列リテラルは `L"..."`、パスは `std::wstring`） |
| ファイル文字コード | UTF-8（BOM付き） |
| インデント | タブ（`.cpp` の一部は4スペース。既存ファイルに合わせる） |

---

## 2. ディレクトリ構成

```
/                 アプリ全体の基盤（Application, Input, Geometry, FileLoader, StringUtility, main）
/Scene            シーン関連（Scene基底, SceneController, 各シーン）
/Game             ゲーム内オブジェクト（Actor派生, Factory, Stage, Collision など）
/Asset            画像・ステージデータ
```

- 1クラス = `.h` と `.cpp` の1ペア。ファイル名はクラス名と同じ（`Player.h` / `Player.cpp`）。
- 別ディレクトリのヘッダは相対パスでインクルードする（`#include"../Application.h"`）。

---

## 3. 命名規則

| 対象 | 規則 | 例 |
|---|---|---|
| クラス・構造体 | PascalCase | `SceneController`, `EnemyFactory`, `Vector2` |
| メンバ関数・グローバル関数 | PascalCase | `Update()`, `GetPosition()`, `IsHit()` |
| メンバ変数 | camelCase + **末尾アンダースコア** | `handle_`, `frame_`, `update_`, `isDead_` |
| ローカル変数・引数 | camelCase | `wsize`, `playerPos`, `newScene` |
| 定数（`constexpr`） | snake_case | `fade_interval`, `player_hit_radius` |
| `enum class` の型名 | PascalCase | `ActorType`, `ShotType` |
| `enum class` の値 | snake_case | `ActorType::player`, `ShotType::reflect_laser` |
| 型エイリアス | PascalCase + `_t` | `UpdateFunc_t`, `Enemies_t`, `InputTable_t` |
| 構造体（POD）の公開メンバ | 短い小文字、アンダースコアなし | `x`, `y`, `w`, `h`, `pos`, `radius` |

### 関数名の接頭辞
- 取得: `Get〇〇()`（`GetWindowSize`, `GetEnemies`）
- 設定: `Set〇〇()`（`SetForce`, `SetBGMVolume`）
- 真偽判定: `Is〇〇()` / `Has〇〇`（`IsDead`, `IsTriggered`, `IsResurrecting`）
- 状態関数: `〇〇Update()` / `〇〇Draw()`（`FadeInUpdate`, `NormalDraw`）
- 画像ハンドルは `handle_` または `〇〇H_`（`backH_`, `indicatorH_`）

> 注意: 既存コードには `playerSpeed`（Player）や `isLockedOn`（Enemy）のように末尾 `_` が抜けたメンバがありますが、
> 新規コードでは **必ず末尾 `_` を付ける** こと。

---

## 4. ヘッダファイルの書き方

```cpp
#pragma once
#include<memory>          // 標準ライブラリ
#include"../Geometry.h"   // 値として使う型のヘッダだけをインクルード

class Player;             // ポインタ・参照でしか使わない型は前方宣言（プロトタイプ宣言）
class EffectFactory;

/// <summary>
/// 敵基底クラス
/// </summary>
class Enemy : public Actor
{
protected:
    std::shared_ptr<EffectFactory> effectFactory_;//エフェクト発生用
    int handle_=-1;//敵の画像ハンドル
public:
    Enemy(int handle, const Position2& pos, ...);
    virtual void OnHit(std::shared_ptr<Actor>)override;
};
```

### ルール
- インクルードガードは `#pragma once`。
- `#include<...>` / `#include"..."` は **`#include` の後にスペースを入れない** スタイル。
- **前方宣言を積極的に使う**。ヘッダでは、ポインタ・参照・スマートポインタでしか使わない型は
  `class Xxx;` で済ませ、実際の `#include` は `.cpp` 側で行う。
- アクセス指定子の順番は **`private`（または `protected`） → `public`**。
  - private 側: メンバ変数 → 型エイリアス → 状態関数・内部ヘルパ
  - public 側: コンストラクタ/デストラクタ → 公開関数
- メンバ変数は **宣言時にデフォルト値を入れる**（`int frame_ = 0;`, `int handle_ = -1;`, `Vector2 vel_ = {};`）。
- 画像ハンドルなど「無効値」の初期値は `-1`。
- getter など状態を変えない関数には `const` を付ける（`bool IsDead()const;`）。
- 派生クラスでオーバーライドする関数には `override` を付ける。
- 引数のオブジェクトは `const T&` で受ける（`const Position2& pos`, `const std::string& code`）。
- デフォルト引数はヘッダの宣言側だけに書く（`const Vector2& offset = {}`）。

---

## 5. コメントの書き方

本プロジェクトは **日本語コメントを非常に丁寧に書く** スタイルです。

### 5.1 公開関数には XML ドキュメントコメント
```cpp
/// <summary>
/// 今、押した瞬間か？(押しっぱなしでは反応しない)
/// </summary>
/// <param name="code">入力コード</param>
/// <returns>true:押した瞬間 / false:押してないor押しっぱなし</returns>
bool IsTriggered(const std::string& code)const;
```
- 真偽値の戻り値は `true:〇〇 / false:〇〇` の形式で意味を書く。
- クラス宣言の直前にも `/// <summary>` でクラスの役割を書く。

### 5.2 メンバ変数・enum 値には行末コメント
```cpp
int resurrectFrame_=0;//復活中フレーム
enum class ItemType {
    speedup,//スピードアップ
    missile,//ミサイル
};
```

### 5.3 処理の「なぜ」を書く
処理内容だけでなく、**そうしている理由・やってはいけないこと** を書く。
```cpp
//ChangeSceneが完了した時点で、自分は死んでいます。
controller_.ChangeScene(std::make_shared<GameScene>(controller_));
//この後に処理を行うと必ず失敗します。なので、速やかにreturnします
return;
```
```cpp
//at関数は存在しないキーを引数として入力されるとクラッシュするため
//事前にキーの存在確認を行う
if (!currentInput_.contains(code)) {
    return false;
}
```

---

## 6. 定数

- マジックナンバーは使わず、**`.cpp` の先頭に `constexpr` で定義** する（ファイルローカル）。
- 名前は snake_case、行末に単位や意味をコメントする。

```cpp
constexpr float player_default_speed = 4.0f;
constexpr int resurrecting_interval = 120;//復活までの時間
constexpr int enemy_img_interval = 5;//アニメーション画像１枚当たり何フレーム表示するのか
constexpr float wave_degree_per_frame = 6.0f;//６度
```

- ファイル内だけで使う構造体などは **無名 namespace** に入れる（`Input.cpp` の `KeyConfigHeader`）。
- `float` リテラルは `1.0f` のように `f` を付ける。

---

## 7. 型・キャスト・関数の使い方

- 型変換は **`static_cast` を使用**（C スタイルキャストは使わない）。
  ```cpp
  float rate = static_cast<float>(frame_) / static_cast<float>(fade_interval);
  ```
- 型が明らかなところは `auto` を使う。取得した参照を書き換えたい場合は `auto&`、読むだけなら `const auto&`。
  ```cpp
  auto& pos = coll_->GetCircle().pos;                          //書き換える
  const auto& wsize = Application::GetInstance().GetWindowSize(); //読むだけ
  ```
- 範囲 for を使う（`for (auto& enemy : enemies_)`）。
- 値の制限は `std::clamp` / `std::min` / `std::max` を使う。
- コンテナからの要素削除は C++20 の `std::erase_if` + ラムダ。
  ```cpp
  std::erase_if(enemies_, [](const auto& enemy) { return enemy->IsDead(); });
  ```
- `std::map` のキー存在確認は `contains()`。`const` 関数内では `[]` ではなく `at()` を使う。
- 列挙型は必ず `enum class`。配列添字に使うときは `static_cast<int>(type)`。
- 座標は `Position2`、方向・速度は `Vector2` と使い分ける（`using Position2 = Vector2;`）。

---

## 8. メモリ管理（スマートポインタ）

| 用途 | 使う型 |
|---|---|
| 所有する（生成した側が持つ） | `std::shared_ptr<T>`（生成は `std::make_shared`） |
| 参照するだけ・循環参照を避けたい | `std::weak_ptr<T>` |
| 自分自身の shared_ptr を渡したい | `std::enable_shared_from_this<T>` を継承して `shared_from_this()` |
| 必ず存在する親（コントローラ等） | 参照 `T&`（例: `SceneController& controller_`） |

- **生ポインタと `new` / `delete` は使わない**（DxLib のファイル操作などやむを得ない場合を除く）。
- 他オブジェクト（プレイヤー、ゲームシーン等）を参照するときは `weak_ptr` で持ち、使う前に `expired()` で確認してから `lock()` する。
  ```cpp
  if (player_.expired()) {
      return;
  }
  const auto& playerPos = player_.lock()->GetPosition();
  ```
- `shared_from_this()` はコンストラクタ内では使えないため、初期化は
  **最初の Update（`InitUpdate`）や `Init()` / `CollisionSet()` で行う**（GameScene, Enemy::Init 参照）。
- 派生型への変換は `std::dynamic_pointer_cast` を使い、結果を null チェックする。

---

## 9. 設計パターン

### 9.1 シングルトン（Application）
アプリに1つしかないものだけに使う。コンストラクタを private にし、コピーを `delete` する。
```cpp
class Application {
private:
    Application();
    Application(const Application&)=delete;
public:
    static Application& GetInstance();
};

Application& Application::GetInstance() {
    static Application instance;
    return instance;
}
```
ウィンドウサイズやローダーなど共通情報は `Application::GetInstance().GetXxx()` で取得する。

### 9.2 状態遷移：メンバ関数ポインタによるステートマシン
シーンや敵・プレイヤーの状態は **if/switch のフラグではなく、メンバ関数ポインタの差し替え** で表現する。
```cpp
// .h
using UpdateFunc_t = void (TitleScene::*)(Input&);
using DrawFunc_t   = void (TitleScene::*)();
UpdateFunc_t update_;
DrawFunc_t draw_;
void FadeInUpdate(Input&);
void NormalUpdate(Input& input);
void FadeDraw();
void NormalDraw();

// .cpp
void TitleScene::Update(Input& input) {
    (this->*update_)(input);
}
void TitleScene::NormalUpdate(Input& input) {
    if (input.IsTriggered("ok")) {
        update_ = &TitleScene::FadeOutUpdate;   // 状態遷移
        draw_   = &TitleScene::FadeDraw;
        frame_  = 0;                            // 遷移時にカウンタを初期化
    }
}
```
- 初期状態はコンストラクタで代入する。
- 現在の状態の判定は関数ポインタの比較で行う（`return update_ == &Player::ResurrectingUpdate;`）。
- 使わない引数は名前を省略してよい（`void FadeInUpdate(Input&)`）。

### 9.3 シーン管理（SceneController）
- すべてのシーンは `Scene` を継承し、`Update(Input&)` と `Draw()`（純粋仮想関数）を実装する。
- コンストラクタで `SceneController&` を受け取り、基底へ渡す。
- 遷移方法:
  - `ChangeScene` … 現在のシーンを置き換える
  - `PushScene` / `PopScene` … ポーズ・設定画面など上に重ねる
  - `ResetScene` … 1つだけの状態に戻す
- **`ChangeScene` を呼んだら即 `return`**（自分自身が破棄されるため）。
- 一時アセットはシーンのデストラクタで `DeleteTemporalAssets()` する。

### 9.4 Factory（生成と一括管理）
同種のオブジェクト（敵・弾・エフェクト・アイテム）は `〇〇Factory` が生成・更新・描画・削除を一括管理する。
- `enum class 〇〇Type` と **生成ラムダの `std::vector`（添字 = 種類）** で生成処理を切り替える。
- 管理リストは `std::list<std::shared_ptr<T>>` に型エイリアスを付ける。
- `Update()` で全要素を更新した後、`IsDead()` なものを `std::erase_if` で削除する。

```cpp
using CreateLambda_t = std::function<std::shared_ptr<Enemy>(int, const Position2&, EnemyType)>;
std::vector<CreateLambda_t> createFuncs_;

createFuncs_.push_back([this](int handle, const Position2& pos, EnemyType type) {
    return std::make_shared<Zako>(handle, pos, player_, effectFactory_, bulletFactory_);
});

void EnemyFactory::CreateEnemy(const Position2& pos, EnemyType type) {
    int enemyTypeIdx = static_cast<int>(type);
    enemies_.push_back(createFuncs_[enemyTypeIdx](enemyImgs_[enemyTypeIdx], pos, type));
    enemies_.back()->Init();
}
```
- 列挙の 0 番を `none`（無効）にする場合は、テーブルの0番目にダミーを入れておく。

### 9.5 Actor（ゲームオブジェクト）
- ゲーム内に配置するものは `Actor` を継承し、`Update()` / `Draw()` / `OnHit()` を実装する。
- 座標は Actor 自身ではなく **`Collision`（当たり判定）が持つ**。`GetPosition()` または `coll_->GetCircle().pos` を参照で取得して書き換える。
- 種別はコンストラクタで `type_ = ActorType::xxx;` を設定し、`OnHit` で相手の `GetActorType()` を見て分岐する。
- 消滅は `isDead_ = true;` にするだけ。実際の削除は Factory が行う。
- 衝突処理は「判定する側（GameScene）」が `IsHit()` で調べ、当たっていれば **お互いの `OnHit()` を呼ぶ**。

### 9.6 アセット読み込み（FileLoader）
- 画像・音は DxLib を直接呼ばず、`Application::GetInstance().GetLoader()->LoadGraph(...)` を使う。
- 同じパスは二重ロードされない（キャッシュ）。
- `AssetType::temporary`（シーン限定）/ `AssetType::continuous`（常駐）を指定する。
- ハンドルの解放は `Asset` のデストラクタで自動的に行う（RAII）。

### 9.7 入力（Input）
- キーを直接見ず、**論理名の文字列** で判定する: `"ok"`, `"back"`, `"pause"`, `"shot"`, `"power"`, `"up"`, `"down"`, `"left"`, `"right"`。
- 押しっぱなし: `IsPressed()` / 押した瞬間: `IsTriggered()`。
- Update 関数は `Input&` を引数で受け取る（グローバルに持たない）。

---

## 10. 関数の書き方

- **早期 return** で条件を外し、ネストを浅く保つ。
  ```cpp
  void Player::OnHit(std::shared_ptr<Actor> actor)
  {
      if (IsDead())return;
      if (IsResurrecting())return;
      if (!actor) {
          KillPlayer();
          return;
      }
      ...
  }
  ```
- ループ内のスキップは `continue`（`if (enemy->IsDead())continue;`）。
- フレームカウンタは `++frame_` / `--frame_` を条件式の中で使ってよい（`if (--frame_ <= 0)`）。
- 割合（0.0～1.0）を計算してアニメーション・フェードに使う。
- 波括弧は「関数定義は次の行」「if/for/switch は同じ行」。
  ```cpp
  void SceneController::PopScene()
  {
      if (scenes_.size() <= 1) {
          return;
      }
      scenes_.pop_back();
  }
  ```
- `switch` には `default:` を書き、各 case は `break;` で終える。
- 引数が多い呼び出しは、意味のまとまりごとに改行して行末コメントを付ける。
  ```cpp
  DrawRectRotaGraph(pos.x, pos.y,//プレイヤーの座標
      player_img_width * imgIndex_, 0,//切り取り左,上
      player_img_width, player_img_height, //切り取り幅,高さ
      player_scale, 0.0f,//画像の拡大率,回転角度
      handle_, true);//ハンドル,透過
  ```
- コンストラクタではメンバ初期化子リストを使い、1行に1メンバずつ書く。
  ```cpp
  EnemyFactory::EnemyFactory(std::weak_ptr<Player> player, ...):
      player_(player),
      effectFactory_(effectFactory),
      bulletFactory_(bulletFactory)
  {
  ```

---

## 11. .cpp ファイルの構成順

1. `#include`（DxLib・標準ライブラリ、自分のヘッダ、依存ヘッダ）
2. `constexpr` 定数 / 無名 namespace
3. private の状態関数（`〇〇Update`, `〇〇Draw`）
4. コンストラクタ・デストラクタ
5. public 関数（`Update`, `Draw`, getter/setter など）

---

## 12. 新しいクラスを追加するときのチェックリスト

- [ ] `.h` / `.cpp` をクラス名で作成し、適切なディレクトリ（`Scene/` or `Game/`）に置いた
- [ ] `#pragma once` と、クラスの `/// <summary>` コメントを書いた
- [ ] 前方宣言で済む型はヘッダでインクルードしていない
- [ ] メンバ変数は末尾 `_` 付き・初期値付き・行末コメント付き
- [ ] マジックナンバーを `.cpp` 先頭の `constexpr`（snake_case）にした
- [ ] 状態があるならメンバ関数ポインタのステートマシンで書いた
- [ ] 所有は `shared_ptr`、参照だけなら `weak_ptr`（使用前に `expired()` チェック）
- [ ] オーバーライドに `override`、状態を変えない関数に `const` を付けた
- [ ] 画像・音は `FileLoader` 経由でロードした
- [ ] 入力は `Input` の論理名で判定した
- [ ] 敵・弾などは対応する Factory の生成テーブルに登録した
