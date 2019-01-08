#include "../header/item.h"
#include "../header/player.h"
#include "../header/enemySpawner.h"
#include <Box2D/Box2D.h>
#include <vector>
#include <map>
#include <iostream>

extern double phisycsUpdateInterval;

EnemySpawner::EnemySpawner(std::vector<Player*> &players, std::vector<b2Vec2> &spawnPositions):
players(players), spawnPositions(spawnPositions){
	Reset();
}

int enemyWaveIncrement = 5;

void EnemySpawner::Reset() {
	currentWave = 0;
	enemiesInWave = 10;
	enemiesSpawned = 0;
	totalEnemiesSpawned = 0;
	spawnRate = 5;
	spawnTimer = 0;
}

void EnemySpawner::Update() {
	spawnTimer += phisycsUpdateInterval;

	if (enemiesSpawned < enemiesInWave && spawnTimer > spawnRate) {
		Spawn();
		return;
	}

	if (IsWaveCleared()) {
		StartNextWave();
	}

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
	for (int i = 0; i < spawnPositions.size(); i++) {
		if (/*provera da li je off screen*/ true) {
			return spawnPositions[i];
		}
	}
}

void EnemySpawner::StartNextWave() {
	currentWave++;
	enemiesInWave += enemyWaveIncrement;
	enemiesSpawned = 0;
	spawnTimer = 0;
}