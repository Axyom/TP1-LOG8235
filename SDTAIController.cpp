// Fill out your copyright notice in the Description page of Project Settings.



#include "SoftDesignTraining.h"
#include "SDTAIController.h"
#include "Engine.h"



void ASDTAIController::Tick(float deltaTime)
{
	UWorld * World = GetWorld();
	auto pawn = GetPawn();

	// hyperparametres
	const float viewDistance = 500;

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	FHitResult losHit;
	FCollisionQueryParams queryParams = FCollisionQueryParams();
	queryParams.AddIgnoredActor(pawn);
	bool collision = GetWorld()->LineTraceSingleByObjectType
	(
		losHit,
		pawn->GetActorLocation(),
		pawn->GetActorLocation() + viewDistance * pawn->GetActorForwardVector(),
		TraceObjectTypes, queryParams
	);

	if (collision)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("MUR"));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT(losHit.ImpactPoint));
	}
	else
	{
		pawn->AddMovementInput(pawn->GetActorForwardVector(), 1., true);
	}
}

void agentOnTheMove()

FVector velocityCalculator(FVector acceleration, float maxSpeed, FVector currentVelocity, float deltaTime) {
	FVector newVelocity = currentVelocity + acceleration * deltaTime;
	if (newVelocity.Size() > maxSpeed) {
		newVelocity.Normalize();
		newVelocity = newVelocity * maxSpeed;
	}
	return newVelocity;
}

/*void default_behaviour()
{
	if ()
	{
		physicHelper.CastRay(selfPosition, targetActor->GetActorLocation(), hitResult, true);
	}
	else if ()
	{
	}
	else
	{
		pawn->AddMovementInput(pawn->GetActorForwardVector(), 1., true);
	}
}*/



/*bool ASDTAIController::MoveToTarget(FVector2D target, float targetSpeed, float deltaTime)
{
APawn* pawn = GetPawn();

FVector const pawnPosition(pawn->GetActorLocation());

FVector2D const toTarget = target - FVector2D(pawnPosition);

FVector2D const displacement = FMath::Min(toTarget.Size(), FMath::Min(m_maxSpeed, targetSpeed) * deltaTime) * toTarget.GetSafeNormal();

pawn->SetActorLocation(pawnPosition + FVector(displacement, 0.f), true);

pawn->SetActorRotation(FVector(displacement, 0.f).ToOrientationQuat());

return toTarget.Size() < FMath::Min(m_maxSpeed, targetSpeed) * deltaTime;

}



void ASDTAIController::MoveTowardsDirection(FVector2D direction, float targetSpeed, float deltaTime)

{

APawn* pawn = GetPawn();

FVector const pawnPosition(pawn->GetActorLocation());

FVector2D const displacement = FMath::Min(m_maxSpeed, targetSpeed) * deltaTime * direction.GetSafeNormal();

pawn->SetActorLocation(pawnPosition + FVector(displacement, 0.f), true);

pawn->SetActorRotation(FVector(direction, 0.f).ToOrientationQuat());

}*/