// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticMeshBatcher.h"
#include "Components/InstancedStaticMeshComponent.h"


// Sets default values
AStaticMeshBatcher::AStaticMeshBatcher()
{
	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>("Test");
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStaticMeshBatcher::BeginPlay()
{
	Super::BeginPlay();
	TArray<FPrimitiveInstanceId> InstanceIds;
	for (int i = 0; i < 10; ++i)
	{
		FTransform T = FTransform();
		T.SetLocation(FVector(0.0,i*100.0,1000.0));
		InstanceIds.Add(InstancedStaticMeshComponent->AddInstanceById(T));
		
	}
	InstancedStaticMeshComponent->RemoveInstanceById(InstanceIds[2]);
	InstancedStaticMeshComponent->RemoveInstanceById(InstanceIds[5]);
	InstancedStaticMeshComponent->RemoveInstanceById(InstanceIds[6]);
}

// Called every frame
void AStaticMeshBatcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

