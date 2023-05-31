// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "SGameplayInterface.h"


// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQuaryParams;
	ObjectQuaryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();

	FVector Eyelocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(Eyelocation,EyeRotation);

	FVector End = Eyelocation + (EyeRotation.Vector()*1000);
	
	// FHitResult Hit;
	// bool bLineBlocked = GetWorld()->LineTraceSingleByObjectType(Hit,Eyelocation,End,ObjectQuaryParams);

	TArray<FHitResult> Hits;
	
	float Radius = 30.0f;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	
	
	bool bLineBlocked = GetWorld()->SweepMultiByObjectType(Hits,Eyelocation,End,FQuat::Identity,ObjectQuaryParams,Shape);

	FColor LineColor = bLineBlocked?FColor::Green:FColor::Red;
	
	for(FHitResult Hit:Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if(HitActor)
		{
			if(HitActor->Implements<USGameplayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);

				ISGameplayInterface::Execute_Interact(HitActor,MyPawn);
				DrawDebugSphere(GetWorld(),Hit.ImpactPoint,Radius,32,LineColor,false,2.0f);
				break;
			}
		}
		
	}
	
	
	DrawDebugLine(GetWorld(),Eyelocation,End,LineColor,false,2.0f,0,2.0f);
}
