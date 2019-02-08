// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "SDTAIController.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = AI, config = Game)
class SOFTDESIGNTRAINING_API ASDTAIController : public AAIController
{
    GENERATED_BODY()
public:
    virtual void Tick(float deltaTime) override;
	FVector velocityCalculator(FVector acceleration, float maxSpeed, FVector currentVelocity);
	int CheckThingsInFrontOfYou(APawn* pawn, UWorld * World, float distanceVision, FVector& hitLocation);
	void rotateBro(APawn* pawn, float angle);
	void rotationInstance(APawn* pawn, float angleRotation);
	void moveToBro(APawn* pawn, FVector target);
	void stateMachine();
private:
	int turnMovement = 5;
	bool isRotating = false;
	int sideRotating = 1;
	int state = 0;
	UPROPERTY(EditAnywhere)
	float maxSpeed = 1.5f;
	UPROPERTY(EditAnywhere)
	float distanceVision = 500.f;
};
