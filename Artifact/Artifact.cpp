#include "Artifact.h"
#include<iostream>

namespace Artifact {
	void Artifact::Upgrade() {
		if (subStatus.size() < 4) {
			ArtifactManager manager = ArtifactManager();
			manager.GenerateSubStatus(*this);
		}
		else {
			int rnd = GenerateRandomNumber(0, 4);
			subStatus[rnd].EnchanceStatus();
		}
	}
	void Artifact::DrawInfo() const {
		cout << type << endl;
		cout << "main : " << mainStatus.name << " " << mainStatus.value << endl;

		for (const auto& sub : subStatus) {
			cout << "sub : " << sub.name << " " << sub.value << endl;
		}

		cout << "score : " << CalcScore() << endl;
	}
	float Artifact::CalcScore() const {
		float score = 0;
		for (const auto& sub : subStatus) {
			if (sub.name == "CriticalRate") {
				score += sub.value * 2.0f;
			}
			if (sub.name == "Attack") {
				score += sub.value;
			}
			if (sub.name == "CriticalDamage") {
				score += sub.value;
			}
		}
		return score;
	}

	ArtifactManager::ArtifactManager() {
		RegisterMain();
		RegisterSub();
	}

	void ArtifactManager::RegisterSub() {
		subStatusDate.push_back({ Status("Attack+",{14,16,18,19}),13.6f });
		subStatusDate.push_back({ Status("Attack",{4.1f,4.7f,5.3f,5.8f}),9.1f });
		subStatusDate.push_back({ Status("Defence+",{16,19,21,23}),13.6f });
		subStatusDate.push_back({ Status("Defence",{5.1f,5.8f,6.6f,7.3f}),9.1f });
		subStatusDate.push_back({ Status("HP+",{209,239,269,299}),13.6f });
		subStatusDate.push_back({ Status("HP",{4.1f,4.7f,5.3f,5.8f}),9.1f });
		subStatusDate.push_back({ Status("Element+",{16,19,21,23}),9.1f });
		subStatusDate.push_back({ Status("Charge",{4.5f,5.2f,5.8f,6.5f}),9.1f });
		subStatusDate.push_back({ Status("CriticalRate",{2.7f,3.1f,3.5f,3.9f}),7.2f });
		subStatusDate.push_back({ Status("CriticalDamage",{5.4f,6.2f,7.0f,7.8f}),6.5f });
	}
	void ArtifactManager::RegisterMain() {

		vector<StatusData> data;
		data.push_back({ Status("HP+",{4780}),100.0f });
		mainStatusDate.emplace("flower", data);
		data.clear();

		data.push_back({ Status("Attack+",{310}),100.0f });
		mainStatusDate.emplace("feather", data);
		data.clear();

		data.push_back({ Status("Attack",{46.6f}),26.4f });
		data.push_back({ Status("HP",{46.6f}),26.4f });
		data.push_back({ Status("Defence",{46.6f}),26.4f });
		data.push_back({ Status("Charge",{51.8f}),10.4f });
		data.push_back({ Status("Element+",{187}),10.4f });
		mainStatusDate.emplace("sands", data);
		data.clear();

		data.push_back({ Status("Attack",{46.6f}),21.6f });
		data.push_back({ Status("HP",{46.6f}),18.6f });
		data.push_back({ Status("Defence",{46.6f}),17.6f });
		data.push_back({ Status("Element+",{187}),3.8f });
		data.push_back({ Status("PhysicsDamageBuf",{58.3f}),4.8f });
		data.push_back({ Status("ElementDamageBuf",{46.6f}),4.8f * 7.0f });
		mainStatusDate.emplace("gobled", data);
		data.clear();

		data.push_back({ Status("Attack",{46.6f}),24.0f });
		data.push_back({ Status("HP",{46.6f}),24.0f });
		data.push_back({ Status("Defence",{46.6f}),22.0f });
		data.push_back({ Status("Element+",{187}),3.0f });
		data.push_back({ Status("HealBuf",{35.9f}),11.0f });
		data.push_back({ Status("CriticalRate",{31.1f}),8.0f });
		data.push_back({ Status("CriticalDamage",{62.2f}),8.0f });
		mainStatusDate.emplace("circlet", data);
	}

	Artifact ArtifactManager::GenerateArtifact(string type) {
		Status mainStatus = Status();
		int rnd = GenerateRandomNumber(0, 1000);
		float rate = mainStatusDate[type][0].rate;

		for (size_t i = 0, size = mainStatusDate[type].size(); i < size; ++i) {
			auto& statusData = mainStatusDate[type][i];
			if (rnd < rate * 10.0f) {
				mainStatus = Status(mainStatusDate[type][i].status);
				mainStatus.EnchanceStatus();
				break;
			}
			else {
				if (i + 1 < mainStatusDate[type].size()) {
					rate += mainStatusDate[type][i + 1].rate;
				}
			}
		}
		Artifact artifact = Artifact(type, mainStatus);

		int subCount = 3;
		if (GenerateRandomNumber(0, 10000) < 3983) {
			subCount = 4;
		}

		for (int i = 0; i < subCount; ++i) {
			GenerateSubStatus(artifact);
		}

		return artifact;
	}

	void ArtifactManager::GenerateSubStatus(Artifact& artifact) {
		string artifactType = artifact.GetType();
		Status main = artifact.GetMainStatus();
		vector<Status> sub = artifact.GetSubStatus();

		float maxRate = 100.0f;
		vector<StatusData> statusList;
		statusList.reserve(subStatusDate.size());
		for (size_t i = 0, size = subStatusDate.size(); i < size; ++i) {
			auto& statusData = subStatusDate[i];
			if (!artifact.IsEnchanted(statusData.status.name)) {
				statusList.push_back(statusData);
			}
			else {
				maxRate -= statusData.rate;
			}
		}
		float rnd = (float)GenerateRandomNumber(0, (int)(maxRate * 10.0f));
		float rate = statusList[0].rate;
		for (size_t i = 0, size = statusList.size(); i < size; i++) {
			if (rnd < rate * 10.0f) {
				Status newSubStatus = Status(statusList[i].status);
				newSubStatus.EnchanceStatus();
				artifact.AddSubStatus(newSubStatus);
				break;
			}
			else {
				if (i + 1 < statusList.size()) {
					rate += statusList[i + 1].rate;
				}
			}
		}

	}
	bool ArtifactManager::CheckRegisterSubStatus(string id) {
		for (const auto& sub : subStatusDate) {
			if (sub.status.name == id) {
				return true;
			}
		}
		return false;
	}
	bool ArtifactManager::CheckRegisterMainStatus(string type,string id) {
		for (const auto& main : mainStatusDate[type]) {
			if (main.status.name == id) {
				return true;
			}
		}
		return false;
	}
	void ArtifactManager::DrawMainStatusVec(string type) {
		for (const auto& data : mainStatusDate) {
			if (data.first != type) continue;
			for (const auto& main : data.second) {
				cout << main.status.name << endl;
			}
			break;
		}
	}
}
