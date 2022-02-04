#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttachment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentOverlap, class ACharacter*, InAttacker, class AActor*, InAttackCauser, class ACharacter*, InOtherCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentCollision);

UCLASS()
class UE_RPG_20_API ACAttachment : public AActor
{
	GENERATED_BODY()

private :
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Root;

public :
	ACAttachment();

	void OnCollision();
	void OffCollision();

protected :
	virtual void BeginPlay() override;

public :
	UFUNCTION(BlueprintImplementableEvent)
		void OnEquip();


	UFUNCTION(BlueprintImplementableEvent)
		void OnUnequip();

protected :
	UFUNCTION(BlueprintCallable)
		void AttachTo(FName InSocketName);

	UFUNCTION(BlueprintCallable)
		void AttachCollision(FName InSocketName);

private :
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected :
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

protected :
	UPROPERTY()
		TArray<class UShapeComponent* > Collisions;

public :
	FAttachmentOverlap OnAttachmentBeginOverlap;
	FAttachmentOverlap OnAttachmentEndOverlap;

	FAttachmentCollision OnAttachmentCollision;
	FAttachmentCollision OffAttachmentCollision;

};
