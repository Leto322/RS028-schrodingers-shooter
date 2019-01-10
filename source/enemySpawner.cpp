#include "../header/item.h"
#include "../header/player.h"
#include "../header/enemySpawner.h"
#include <Box2D/Box2D.h>
#include <vector>
#include <map>
#include <iostream>
#include <time.h>


extern double phisycsUpdateInterval;

EnemySpawner::EnemySpawner(std::vector<Player*> &players, std::vector<b2Vec2> &spawnPositions):
players(players), spawnPositions(spawnPositions){
	Reset();
}

int enemyWaveIncrement = 2;

void EnemySpawner::Reset() {
	currentWave = 0;
	enemiesInWave = 5;
	enemiesSpawned = 0;
	totalEnemiesSpawned = 0;
	spawnRate = 5; //Seconds
	spawnTimer = 0;
}

void EnemySpawner::Update() {
	spawnTimer += phisycsUpdateInterval;

	if (enemiesSpawned < enemiesInWave && spawnTimer > spawnRate) {
		Spawn();
		return;
	}

	if (IsWaveCleared()) {
		std::cout << "Wave " << currentWave << " cleared!" << std::endl;
		StartNextWave();
	}

}

int EnemySpawner::GetEnemiesInWave() const{
	return enemiesInWave;
}
int EnemySpawner::GetEnemiesSpawned() const{
	return enemiesSpawned;
}
int EnemySpawner::GetCurrentWave() const{
	return currentWave;
}

bool EnemySpawner::IsWaveCleared() {
	int numberOfAliveBots = 0;
	for (int i = 1; i < players.size(); i++) {
		if (players[i]->alive)
			numberOfAliveBots++;
	}
	return (numberOfAliveBots == 0 && enemiesSpawned == enemiesInWave);
}

void EnemySpawner::Spawn() {
	spawnTimer = 0;
	for (int i = 1; i < players.size(); i++) {
		if (!players[i]->alive) {
			players[i]->Revive();
			b2Vec2 spawnPosition = GetNextSpawnPosition();
			players[i]->body->SetTransform(spawnPosition, 0);

			enemiesSpawned++;
			totalEnemiesSpawned++;
			return;
		}
	}
}

b2Vec2 EnemySpawner::GetNextSpawnPosition() {
	while (true) {
		int index = rand() % spawnPositions.size();
		if (!IsOnScreen(spawnPositions[index])) {
			return spawnPositions[index];
		}
	}
}

void EnemySpawner::StartNextWave() {
	currentWave++;
	enemiesInWave += enemyWaveIncrement;
	enemiesSpawned = 0;
	spawnTimer = 0;
}
