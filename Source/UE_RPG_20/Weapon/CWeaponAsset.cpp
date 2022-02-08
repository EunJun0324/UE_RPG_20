#include "Weapon/CWeaponAsset.h"
#include "Global.h"
#include "CAttachment.h"
#include "CEquippment.h"
#include "CDoAction.h"
#include "CSubAction.h"
#include "GameFramework/Character.h"

UCWeaponAsset::UCWeaponAsset()							
{
	CHelpers::GetClass<UCEquippment>(&EquipmentClass, "Blueprint'/Game/Weapons/BP_CEquipment.BP_CEquipment_C'");

	DoActionClass  = UCDoAction::StaticClass();
	SubActionClass = UCSubAction::StaticClass();
}

void UCWeaponAsset::BeginPlay(class ACharacter* InOwner)
{
	if (AttachmentClass)
	{
		FActorSpawnParameters params;
		params.Owner = InOwner;

		Attachment = InOwner->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass, params);
	}
	if (EquipmentClass)
	{
		Equippment = NewObject<UCEquippment>(this, EquipmentClass);
		Equippment->BeginPlay(InOwner, EquipmentData);

		if (Attachment)
		{
			Equippment->OnEquip.AddDynamic(Attachment, &ACAttachment::OnEquip);
			Equippment->OnUnequip.AddDynamic(Attachment, &ACAttachment::OnUnequip);
		}
	}
	if (DoActionClass)
	{
		DoAction = NewObject<UCDoAction>(this, DoActionClass);
		DoAction->BeginPlay(Attachment, InOwner, DoActionDatas, HitDatas);

		if (Equippment)
		{
			DoAction->SetEquipping(Equippment->IsEquipping());
			Equippment->OnEquip.AddDynamic(DoAction, &UCDoAction::OnEquip);
			Equippment->OnUnequip.AddDynamic(DoAction, &UCDoAction::OnUnequip);
		}
		if (Attachment)
		{
			Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);
			Attachment->OnAttachmentEndOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndOverlap);

			Attachment->OnAttachmentCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentCollision);
			Attachment->OffAttachmentCollision.AddDynamic(DoAction, &UCDoAction::OffAttachmentCollision);
		}
	}

	if (SubActionClass)
	{
		SubAction = NewObject<UCSubAction>(this, SubActionClass);
		SubAction->BeginPlay(InOwner);

		if (Attachment)
		{
			Attachment->OnAttachmentBeginOverlap.AddDynamic(SubAction, &UCSubAction::OnAttachmentBeginOverlap);
			Attachment->OnAttachmentEndOverlap.AddDynamic(SubAction, &UCSubAction::OnAttachmentEndOverlap);

			Attachment->OnAttachmentCollision.AddDynamic(SubAction, &UCSubAction::OnAttachmentCollision);
			Attachment->OffAttachmentCollision.AddDynamic(SubAction, &UCSubAction::OffAttachmentCollision);
		}
	}
}

void UCWeaponAsset::EndPlay()
{
}
