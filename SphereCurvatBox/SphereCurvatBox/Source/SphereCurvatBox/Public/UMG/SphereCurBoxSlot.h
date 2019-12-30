// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"
#include "Layout/Margin.h"
#include "UMG/SphereCurBox.h"
#include "Components/PanelSlot.h"

#include "SphereCurBoxSlot.generated.h"

/**
 * 
 */
UCLASS()
class SPHERECURVATBOX_API USphereCurBoxSlot : public UPanelSlot
{
	GENERATED_UCLASS_BODY()

public:

	/** The padding area between the slot and the content it contains. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout|SphereCurBox Slot")
		FMargin Padding;

	/** The alignment of the object horizontally. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout|SphereCurBox Slot")
		TEnumAsByte<EHorizontalAlignment> HorizontalAlignment;

	/** The alignment of the object vertically. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout|SphereCurBox Slot")
		TEnumAsByte<EVerticalAlignment> VerticalAlignment;

public:

	UFUNCTION(BlueprintCallable, Category = "Layout|SphereCurBox Slot")
		void SetPadding(FMargin InPadding);

	UFUNCTION(BlueprintCallable, Category = "Layout|SphereCurBox Slot")
		void SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment);

	UFUNCTION(BlueprintCallable, Category = "Layout|SphereCurBox Slot")
		void SetVerticalAlignment(EVerticalAlignment InVerticalAlignment);

public:

	// UPanelSlot interface
	virtual void SynchronizeProperties() override;
	// End of UPanelSlot interface

	/** Builds the underlying FSlot for the Slate layout panel. */
	void BuildSlot(TSharedRef<SSphereCurBox> ScrollBox);

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
	//TODO UMG Slots should hold weak or shared refs to slots.

	/** A raw pointer to the slot to allow us to adjust the size, padding...etc at runtime. */
	SSphereCurBox::FSlot* Slot;
};
