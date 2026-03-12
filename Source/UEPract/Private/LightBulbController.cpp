// Fill out your copyright notice in the Description page of Project Settings.


#include "LightBulbController.h"

ALightBulbController::ALightBulbController() {
	PrimaryActorTick.bCanEverTick = true;
}

void ALightBulbController::ToggleLight() {
	is_enabled ? SetEnabled(false) : SetEnabled(true);
}
