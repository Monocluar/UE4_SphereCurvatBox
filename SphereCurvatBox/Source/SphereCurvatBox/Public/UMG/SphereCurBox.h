// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Styling/SlateTypes.h"
#include "Slate/SSphereCurBox.h"
#include "Components/PanelWidget.h"
#include "SphereCurBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUserSphereScrolledEvent, float, CurrentOffset);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUserSphereAlignedScrolledEvent, UWidget*, Content);
/**
 * 
 */
UENUM(BlueprintType)
enum class ESphereCurBoxScrollKey : uint8
{
	LeftMouse UMETA(DisplayName = "鼠标左键"),

	RightMouse UMETA(DisplayName = "鼠标右键"),

	/** 依照控件下边生成 */
	SAlign_Bottom UMETA(DisplayName = "鼠标中键"),

	/** 依照控件右边生成 */
	SAlign_Right UMETA(DisplayName = "Right"),
};


class USphereCurBoxSlot;

UCLASS()
class SPHERECURVATBOX_API USphereCurBox : public UPanelWidget
{
	GENERATED_UCLASS_BODY()
	
public:

	/** 自身风格设置 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style", meta = (DisplayName = "Style"))
		FScrollBoxStyle WidgetStyle;

	/** 下拉栏风格设置 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style", meta = (DisplayName = "Bar Style"))
		FScrollBarStyle WidgetBarStyle;

	UPROPERTY()
		USlateWidgetStyleAsset* Style_DEPRECATED;

	UPROPERTY()
		USlateWidgetStyleAsset* BarStyle_DEPRECATED;

	/** 下拉栏可视性 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scroll")
		ESlateVisibility ScrollBarVisibility;

	/** 下拉栏大小设置 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scroll")
		FVector2D ScrollbarThickness;

	/** 滑动栏启动的建 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scroll")
		FKey MouseButtonScrollKey;

	/** 是否一直显示下拉栏 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scroll")
		bool bAlwaysShowScrollbar;

	/** 是否禁用惯性滚动 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scroll")
		bool bAllowOverscroll;

	/** 圆的折叠方向 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sphere")
		ESpherePosAlignment SpherePosAlignment;

	/** 圆的半径*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sphere", meta = (UIMin = "10"))
		float SphereRadius;

	/** 控件间隔距离 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sphere", meta = (UIMin = "1"))
		float WidgetIntervalPos;

	/** 是否循环 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sphere")
	bool bLoopChildren;

	/** 滑块是否对齐滑动 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sphere")
	bool bAlignedWidget;

	/** 子控件整体重心位置 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sphere")
		FVector2D ChildrenAlignment;


	

public:
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
		FOnUserSphereScrolledEvent OnUserScrolled;

	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnUserSphereAlignedScrolledEvent OnUserAlignedScrolled;

	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnUserSphereAlignedScrolledEvent OnCenterAlignedScrolled;

public:
	UFUNCTION(BlueprintCallable, Category = "Widget")
		void ScrollWidgetIntoView(UWidget* WidgetToFind, bool AnimateScroll = true);

	UFUNCTION(BlueprintCallable, Category = "Widget")
		void ScrollWidgetIntoWidget(UWidget* WidgetToFind, UWidget* IntoWidget, bool AnimateScroll = true);

	UFUNCTION(BlueprintCallable, Category = "Widget")
		void ScrollWidgetIntoOffice(float NewScrollOffset, bool AnimateScroll = true);

	UFUNCTION(BlueprintCallable, Category = "Widget")
		void SetScrollOffset(float NewScrollOffset);

	UFUNCTION(BlueprintCallable, Category = "Widget")
		void ScrollToStart();

	UFUNCTION(BlueprintCallable, Category = "Widget")
		void ScrollToEnd();

	UFUNCTION(BlueprintCallable, Category = "Panel")
		USphereCurBoxSlot* AddChildToSphereCurBox(UWidget* Content);

public:

	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

		//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

	//~ Begin UObject Interface
	virtual void PostLoad() override;
	//~ End UObject Interface

#if WITH_EDITOR
	//~ Begin UWidget Interface
	virtual const FText GetPaletteCategory() override;

	//~ End UWidget Interface
#endif

protected:
	virtual UClass* GetSlotClass() const override;
	virtual void OnSlotAdded(UPanelSlot* Slot) override;
	virtual void OnSlotRemoved(UPanelSlot* Slot) override;

	void SlateHandleUserScrolled(float CurrentOffset);

	void SlateHandleUserAlignedScrolled(TSharedPtr<SWidget> Content);

	void SlateHandleCenterAlignedScrolled(TSharedPtr<SWidget> Content);

protected:
	/** The desired scroll offset for the underlying scrollbox.  This is a cache so that it can be set before the widget is constructed. */
	float DesiredScrollOffset;

	TSharedPtr<class SSphereCurBox> MySphereCurBox;

protected:
	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
};
