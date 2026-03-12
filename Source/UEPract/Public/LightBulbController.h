// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightBulbController.generated.h"

UCLASS()
class UEPRACT_API ALightBulbController : public AActor
{
	GENERATED_BODY()
	
public:
	ALightBulbController();
	
	UFUNCTION(BlueprintCallable, Category="LightBulb")
	void SetEnabled(bool enabled) { is_enabled = enabled; }
	UFUNCTION(BlueprintPure, Category="LightBulb")
	bool GetEnabled() const { return is_enabled; }
	UFUNCTION(BlueprintCallable, Category="LightBulb")
	void ToggleLight();

	
	UPROPERTY(BlueprintReadWrite, Category="LightBulb", meta=(AllowPrivateAccess="true"))
	float ENABLED_INTENSITY = 5000.f;
	UPROPERTY(BlueprintReadWrite, Category="LightBulb", meta=(AllowPrivateAccess="true"))
	float DISABLED_INTENSITY = 0.f;
private:
	UPROPERTY(BlueprintReadOnly, Category="LightBulb", meta=(AllowPrivateAccess="true"))
	bool is_enabled = false;
};
