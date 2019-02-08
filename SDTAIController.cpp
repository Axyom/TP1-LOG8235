// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "SDTAIController.h"
#include "SDTCollectible.h"
#include "SoftDesignTrainingMainCharacter.h"
#include "Engine.h"
#include <iostream>
#include <string>
#include <regex>

void ASDTAIController::Tick(float deltaTime)
{
	
	APawn* pawn = GetPawn();


UPROPERTY()
	float angleRotation = 45.f;



	
	
	if (this->isRotating == true) {
		
		rotationInstance(pawn, angleRotation);
	}
	else {
		stateMachine();
	}
	
}

void ASDTAIController::stateMachine() {
	UWorld * World = GetWorld();
	APawn* pawn = GetPawn();
	FVector hitLocation;



	this->state = CheckThingsInFrontOfYou(pawn, World, this->distanceVision, hitLocation);

	switch (this->state) {

	case 0:
		//RunDown
		moveToBro(pawn, pawn->GetActorForwardVector());
		this->state = 0;
		break;
	case 1: {
		//Is there a wall or a death plate ? avoid it randomly

		int rand = FMath::RandRange(0, 1);
		if (rand == 1) {
			this->sideRotating = -this->sideRotating;
		}
		this->isRotating = true;
		this->state = 1;
		break;
	}
	case 2:
		//PickUp Detected ? go for it
		moveToBro(pawn, hitLocation);
		this->state = 2;
		break;

	case 3:
		//It a player ! grab him or run!
		moveToBro(pawn, hitLocation);
		this->state = 3;
		break;
	case 4:
		//It a player !  run!
		moveToBro(pawn, hitLocation);
		this->isRotating = true;
		this->state = 3;
		break;
	}
}
void ASDTAIController::rotateBro(APawn* pawn, float angle) {
	FRotator rot = FRotator(0, angle, 0);
	pawn->AddActorWorldRotation(rot);
}
void ASDTAIController::rotationInstance(APawn* pawn, float angleRotation) {
	float rot;
	//Wait and decomposing rotation
	if (this->state == 1) {
		//Rotating because of a wall
		rot = angleRotation;
	}
	if (this->state == 3) {
		//half turn cause running from the poweredUp Player
		rot = 180;

	}
	this->turnMovement--;
	rotateBro(pawn, this->sideRotating*rot / 5);
	if (this->turnMovement == 0) {
		this->turnMovement = 5;
		this->isRotating = false;
	}
}
void ASDTAIController::moveToBro(APawn* pawn, FVector target) {
	FVector mouvement = target;
	if(pawn->GetActorForwardVector() != target) {
		mouvement = target - pawn->GetActorForwardVector();
	}
	
	
	FVector newMovement = velocityCalculator(mouvement, this->maxSpeed, pawn->GetMovementInputVector());
	pawn->AddMovementInput(newMovement, 1.f, true);
	
}
int ASDTAIController::CheckThingsInFrontOfYou(APawn* pawn, UWorld * world, float distanceVision,FVector& hitLocation) {
	TArray < struct FHitResult >  OutHits;
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	//TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	FCollisionQueryParams queryParams = FCollisionQueryParams();
	queryParams.AddIgnoredActor(pawn);
	
	FCollisionShape shape = FCollisionShape();
	shape.MakeBox(FVector(distanceVision,distanceVision,distanceVision));

	//Checking for Wall
	world->SweepMultiByObjectType(OutHits, pawn->GetActorLocation(), pawn->GetActorLocation() + distanceVision/3 * pawn->GetActorForwardVector(), FQuat::Identity, TraceObjectTypes, shape, queryParams);
	if (OutHits.Num() != 0) {
		//I see a Wall there
		if (OutHits[0].GetActor()->GetHumanReadableName().Find(FString("Wall")) == 0) {
			
			return 1;
		}
	}
	
	//Checking For player or Collectible
	world->SweepMultiByObjectType(OutHits, pawn->GetActorLocation(), pawn->GetActorLocation() + distanceVision * pawn->GetActorForwardVector(), FQuat(ForceInitToZero), TraceObjectTypes, shape, queryParams);
	if (OutHits.Num() != 0) {
		if (ASoftDesignTrainingMainCharacter* mainActor = Cast<ASoftDesignTrainingMainCharacter>(OutHits[0].GetActor())) {

			//i see a human there
			//Check if he is powered Up, in this cas run away
			if (mainActor->IsPoweredUp()) {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("RUN"));
				hitLocation = -pawn->GetActorForwardVector();
				return 4;
			}
			else {
				hitLocation = pawn->GetActorForwardVector();
				return 3;
			}
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, hitLocation.ToCompactString());

		}

		if (ASDTCollectible* collectibleActor = Cast<ASDTCollectible>(OutHits[0].GetActor())) {
			//I see a collectible there
			//Is it visible
			if (!collectibleActor->IsOnCooldown()) {
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Jevoisuncollectible"));
				hitLocation = OutHits[0].GetActor()->GetActorLocation();
				return 2;
			}
			
		}
		
	}
	//Checking For DeathFloor so I look at the floor in front of the player
	TArray < struct FHitResult > floor;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3));
	world->LineTraceMultiByObjectType(floor, pawn->GetActorLocation(), pawn->GetActorLocation() + distanceVision / 2  * pawn->GetActorForwardVector() + FVector(0, 0, -100.f), TraceObjectTypes, queryParams);
	if (floor.Num() != 0) {
		if (floor[0].GetActor()->GetHumanReadableName().Find(FString("BP_Death")) == 0) {
			//We identify a DeathFloor in front of the Pawn
			return 1;
		}
	}

	hitLocation = pawn->GetActorForwardVector();
	return 0;
}


FVector ASDTAIController::velocityCalculator(FVector acceleration, float maxSpeed, FVector currentVelocity) {
	FVector newVelocity = currentVelocity + acceleration;
	if (newVelocity.Size() > maxSpeed) {
		newVelocity.Normalize();
		newVelocity = newVelocity * maxSpeed;
	}	
	return newVelocity;
}

