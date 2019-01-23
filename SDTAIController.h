// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SDTAIController.generated.h"

class USplineComponent;
class PhysicsHelpers;

struct CollisionInfo
{
	CollisionInfo(float timeToCollision, FVector2D const& collisionPosition)
		: m_timeToCollision(timeToCollision)
		, m_collisionPosition(collisionPosition)
	{

	}

	float m_timeToCollision;
	FVector2D m_collisionPosition;
};

UCLASS()
class SOFTDESIGNTRAINING_API ASDTAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ASDTAIController();

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Tick(USplineComponent const& spline, float deltaTime);

protected:
	enum class Stage
	{
		ReachSpline,
		GetAroundObstacle,
		FollowSpline,
		DynamicObstacle,
		Idle
	};

	struct HitInformation
	{
		void ReceivedHit(FVector2D hitNormal)
		{
			m_hitNormal = hitNormal;
			m_hitHandled = false;
		}

		bool HandleHit()
		{
			if (!m_hitHandled)
			{
				m_hitHandled = true;
				return true;
			}

			return false;
		}

		bool m_hitHandled = true;
		FVector2D m_hitNormal;
	};

	virtual bool MoveToTarget(FVector2D target, float speed, float deltaTime);
	virtual void MoveTowardsDirection(FVector2D direction, float speed, float deltaTime);
	virtual bool ReachSpline(USplineComponent const& spline, float deltaTime);
	virtual bool FollowSpline(USplineComponent const& spline, float deltaTime);
	virtual bool GetAroundObstacle(float deltaTime);
	virtual float GetDistanceToSpline(USplineComponent const& spline);
	virtual bool FollowSplineInDynamicObstacles(USplineComponent const& spline, float deltaTime);
	virtual USplineComponent* GetClosestSplineWithoutObstacle(TArray<USplineComponent*> const& splines) const;
	virtual TArray<CollisionInfo> RecoverIncomingCollisions(FVector2D projectedVelocity) const;
	virtual FVector2D FindSplineClosestPosition(USplineComponent const& spline) const;
	virtual TArray<FOverlapResult> CollectTargetActorsInFrontOfCharacter(APawn const * pawn, PhysicsHelpers & physicHelper) const;
	void NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit);

	FVector2D m_MovementInput;
	FVector2D m_StartingPosition;
	float m_capsuleRadius;
	float const m_maxSpeed = 500.0f;
	float const m_maxAcceleration = 500.0f;
	float const m_visionAngle = PI / 3.0f;
	Stage m_stage = Stage::ReachSpline;
	HitInformation m_hitInformation;
	USplineComponent* m_chosenSpline = nullptr;
};
