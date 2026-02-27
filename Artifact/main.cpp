
#include <iostream>
#include <time.h>
#include <thread>
#include <mutex>
#include <chrono>
#include "Artifact.h"

static chrono::steady_clock::time_point start;

namespace Artifact {
    /// <summary>
    /// 目標データ
    /// </summary>
    struct TargetStatus {
        float score;    //スコア
        vector<string> mainStatus;  //メインステータス

        /// <summary>
        /// 指定した聖遺物が目標に達しているか判定
        /// </summary>
        /// <param name="artifact">聖遺物</param>
        /// <returns>判定</returns>
        bool CheckTarget(Artifact& artifact) const {
            if (artifact.CalcScore() < score) {
                return false;
            }
            bool isMatch = false;
            for (const auto& status : mainStatus) {
                if (artifact.GetMainStatus().name == status) {
                    isMatch = true;
                    break;
                }
            }
            return isMatch;
        }
    };
    /// <summary>
    /// 指定した聖遺物を最大値まで強化
    /// </summary>
    /// <param name="artifact">聖遺物</param>
    void MaxUpgrede(Artifact& artifact) {
        int upgrede = 5;
        for (int i = 0; i < upgrede; ++i) {
            artifact.Upgrade();
        }
    }
    /// <summary>
    /// 目標が達成するまで聖遺物を生成する。
    /// 試行回数が最大に達した場合、目標を達成した場合に結果を出力
    /// </summary>
    /// <param name="manager">マネージャー</param>
    /// <param name="artifactType">部位名</param>
    /// <param name="target">目標データ</param>
    /// <param name="maxCount">最大試行回数</param>
    void CalcTargetArtifact(ArtifactManager& manager,string artifactType, const TargetStatus& target, const int maxCount) {
        Artifact artifact;
        float score = 0;
        int count = 0;
        const int reportCount = 10000;
        string mainStatus = "";
        do {
            if (count >= maxCount) {
                cout << "出ませんでした" << endl;
                break;
            }
            artifact = manager.GenerateArtifact(artifactType);
            MaxUpgrede(artifact);
            score = artifact.CalcScore();
            mainStatus = artifact.GetMainStatus().name;
            count++;
            if (count % reportCount == 0) {
                auto end = std::chrono::high_resolution_clock::now();
                float duration = (float)std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
                cout << count << "回演算しました : " << count / duration << "回/s" << endl;
            }
        } while (!target.CheckTarget(artifact));

        if (count <= maxCount) {
            artifact.DrawInfo();
            cout << endl;
            cout << count << "回目で出ました";
        }
    }
    /// <summary>
    /// すべての部位を生成
    /// </summary>
    /// <param name="manager">マネージャー</param>
    void GenerateAllEquipment(ArtifactManager& manager) {

        vector<Artifact> artifacts;
        artifacts.push_back(manager.GenerateArtifact("flower"));
        artifacts.push_back(manager.GenerateArtifact("feather"));
        artifacts.push_back(manager.GenerateArtifact("sands"));
        artifacts.push_back(manager.GenerateArtifact("gobled"));
        artifacts.push_back(manager.GenerateArtifact("circlet"));

        for (auto& artifact : artifacts) {
            MaxUpgrede(artifact);
            artifact.DrawInfo();
            cout << endl;
        }
    }
}

using namespace Artifact;
int main()
{
    /*
    * 花 : flower
    * 羽 : feather
    * 砂 : sands
    * 杯 : gobled
    * 冠 : circlet
    */

    ArtifactManager manager = ArtifactManager();
    int mode = 0;
    cout << "生成方法を選んでください 0: すべての部位を生成する  1: 指定した条件を生成できる回数を求める" << endl;
    cout << ">> ";
    cin >> mode;
    cout << endl;

    switch (mode) {
    case 0:
        GenerateAllEquipment(manager);
        break;
    case 1:
        float score;
        string type;
        string status;
        vector<string> statues = {};
        cout << "目標スコアを設定してください" << endl;
        cout << ">> ";
        cin >> score;
        cout << endl;

        cout << "一覧 : flower: 花  feather: 羽  sands: 砂  gobled: 杯  criclet: 冠" << endl;
        cout << "生成する部位を指定してください" << endl;
        cout << ">> ";
        cin >> type;
        cout << endl;

        do {
            cout << "メインステータスを指定してください。生成開始 : f / 設定可能ステータス一覧表示 : c" << endl;
            cout << ">> ";
            cin >> status;
            if (status != "f" && status != "c") {
                if (manager.CheckRegisterMainStatus(type,status)) {
                    statues.push_back(status);
                    cout << status << "が追加されました" << endl;
                }
                else {
                    cout << "登録されていないステータスです" << endl;
                    continue;
                }
            }
            else if(status == "c"){
                manager.DrawMainStatusVec(type);
            }
            cout << endl;
        } while (status != "f");
        start = std::chrono::high_resolution_clock::now();
        CalcTargetArtifact(manager, type, { score,statues }, INT_MAX);
        break;
    }
}


