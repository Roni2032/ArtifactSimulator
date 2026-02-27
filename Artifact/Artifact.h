#pragma once
#include<vector>
#include<string>
#include <map>
#include <time.h>
#include <random>

using namespace std;

namespace Artifact {
	/// <summary>
	/// 乱数生成
	/// </summary>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <returns>乱数</returns>
	static int GenerateRandomNumber(int min, int max) {
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> dist(min, max - 1);
		return dist(gen);
	}

	class ArtifactManager;
	class StatusData;
	/// <summary>
	/// ステータス
	/// 聖遺物一つにつきメイン１つサブ4つを持ちます
	/// </summary>
	class Status {
	public:
		string name;	//名前
		float value;	//現在の値
		vector<float> updateValue;	//強化時の上昇量
		Status() {
			name = "";
			value = 0;
			updateValue = {};
		}
		Status(string _name, vector<float> _update) {
			name = _name;
			updateValue = _update;
			value = 0;
		}
		~Status() {}
		/// <summary>
		/// 設定されている上昇量からランダムでステータスを強化
		/// </summary>
		void EnchanceStatus() {
			int rnd = GenerateRandomNumber(0, (int)updateValue.size());
			value += updateValue[rnd];
		}
	};

	/// <summary>
	/// 聖遺物本体
	/// </summary>
	class Artifact {
		string type;//部位名

		Status mainStatus; //メインステータス
		vector<Status> subStatus;//サブステータス配列
	public:
		Artifact(const Artifact& artifact) {
			type = artifact.type;
			mainStatus = artifact.mainStatus;
			subStatus = artifact.subStatus;
		}
		Artifact(string _type = "", Status _main = Status()) : mainStatus(_main) {
			type = _type;
		}
		~Artifact() {}
		/// <summary>
		/// 指定したステータスがすでに付けられているか
		/// </summary>
		/// <param name="name">ステータス名</param>
		/// <returns>ステータスの有無</returns>
		bool IsEnchanted(string name) {
			if (mainStatus.name == name) {
				return true;
			}
			for (const auto& sub : subStatus) {
				if (sub.name == name) {
					return true;
				}
			}
			return false;
		}

		/// <summary>
		/// 部位名の取得
		/// </summary>
		/// <returns>部位名</returns>
		string GetType() {
			return type;
		}
		/// <summary>
		/// メインステータス名を取得
		/// </summary>
		/// <returns>メインステータス名</returns>
		Status GetMainStatus() {
			return mainStatus;
		}
		/// <summary>
		/// サブステータス名をすべて取得
		/// </summary>
		/// <returns>サブステータス名</returns>
		vector<Status> GetSubStatus() {
			return subStatus;
		}
		/// <summary>
		/// サブステータスを追加
		/// </summary>
		/// <param name="_status">ステータス名</param>
		void AddSubStatus(Status _status) {
			subStatus.push_back(_status);
		}
		/// <summary>
		/// 聖遺物を一回強化
		/// </summary>
		void Upgrade();
		/// <summary>
		/// 聖遺物の情報を出力
		/// </summary>
		void DrawInfo()const;
		/// <summary>
		/// 聖遺物のスコアを計算
		/// </summary>
		/// <returns>スコア</returns>
		float CalcScore()const;

	};


	/// <summary>
	/// ステータスデータ
	/// </summary>
	struct StatusData {
		Status status;	//ステータス
		float rate;		//発生確率
	};
	class ArtifactManager {
		vector<StatusData> subStatusDate;
		map<string, vector<StatusData>> mainStatusDate;

		/// <summary>
		/// サブステータスを登録
		/// </summary>
		void RegisterSub();
		/// <summary>
		/// メインステータスを登録
		/// </summary>
		void RegisterMain();

	public:
		ArtifactManager(const ArtifactManager& manager) {
			subStatusDate = manager.subStatusDate;
			mainStatusDate = manager.mainStatusDate;
		}
		ArtifactManager();
		~ArtifactManager() {}

		/// <summary>
		/// 聖遺物全体を生成
		/// </summary>
		/// <param name="type">部位名</param>
		/// <returns>生成した聖遺物</returns>
		Artifact GenerateArtifact(string type);
		/// <summary>
		/// 指定した聖遺物のサブステータスを生成
		/// </summary>
		/// <param name="artifact">聖遺物</param>
		void GenerateSubStatus(Artifact& artifact);

		/// <summary>
		/// 指定したステータスがサブに登録されているか判定
		/// </summary>
		/// <param name="id">ステータス名</param>
		/// <returns>判定</returns>
		bool CheckRegisterSubStatus(string id);
		/// <summary>
		/// 指定したステータスがメインに登録されているか判定
		/// </summary>
		/// <param name="type">部位</param>
		/// <param name="id">ステータス名</param>
		/// <returns>判定</returns>
		bool CheckRegisterMainStatus(string type, string id);
		/// <summary>
		/// 指定した部位に登録されているメインステータス名を出力
		/// </summary>
		/// <param name="type">部位名</param>
		void DrawMainStatusVec(string type);
	};
}

