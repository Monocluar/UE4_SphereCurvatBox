// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/SphereCurBox.h"
#include "Containers/Ticker.h"
#include "UMG/SphereCurBoxSlot.h"

#define LOCTEXT_NAMESPACE "UMG"

USphereCurBox::USphereCurBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, ScrollBarVisibility(ESlateVisibility::Visible)
	, ScrollbarThickness(FVector2D(5.0f, 5.0f))
	, MouseButtonScrollKey(EKeys::RightMouseButton)
	, bAlwaysShowScrollbar(false)
	, SpherePosAlignment(ESpherePosAlignment::SAlign_Left)
	, SphereRadius(1000.f)
	, WidgetIntervalPos(10.f)
	, bLoopChildren(false)
	, bAlignedWidget(false)
	, ChildrenAlignment(FVector2D::ZeroVector)

{
	bIsVariable = false;

	SSphereCurBox::FArguments Defaults;
	Visibility = UWidget::ConvertRuntimeToSerializedVisibility(Defaults._Visibility.Get());
	Clipping = EWidgetClipping::ClipToBounds;

	WidgetStyle = *Defaults._Style;
	WidgetBarStyle = *Defaults._ScrollBarStyle;
}

void USphereCurBox::ScrollWidgetIntoView(UWidget * WidgetToFind, bool AnimateScroll)
{
	TSharedPtr<SWidget> SlateWidgetToFind;
	if (WidgetToFind)
	{
		SlateWidgetToFind = WidgetToFind->GetCachedWidget();
	}

	if (MySphereCurBox.IsValid())
	{
		// NOTE: Pass even if null! This, in effect, cancels a request to scroll which is necessary to avoid warnings/ensures 
		//       when we request to scroll to a widget and later remove that widget!
		MySphereCurBox->ScrollDescendantIntoView(SlateWidgetToFind, AnimateScroll,0.f);
	}
}

void USphereCurBox::ScrollWidgetIntoWidget(UWidget * WidgetToFind, UWidget * IntoWidget, bool AnimateScroll)
{
	TSharedPtr<SWidget> SlateWidgetToFind;
	TSharedPtr<SWidget> IntoSlateWidget;
	if (WidgetToFind && IntoWidget)
	{
		SlateWidgetToFind = WidgetToFind->GetCachedWidget();
		IntoSlateWidget = IntoWidget->GetCachedWidget();
	}

	if (MySphereCurBox.IsValid())
	{
		// NOTE: Pass even if null! This, in effect, cancels a request to scroll which is necessary to avoid warnings/ensures 
		//       when we request to scroll to a widget and later remove that widget!
		MySphereCurBox->ScrollDescendantIntoWidget(SlateWidgetToFind, IntoSlateWidget, AnimateScroll);
	}
}

void USphereCurBox::ScrollWidgetIntoOffice(float NewScrollOffset, bool AnimateScroll)
{
	if (MySphereCurBox.IsValid())
	{
		MySphereCurBox->ScrollDescendantIntoOffset(NewScrollOffset, AnimateScroll);
	}
}

void USphereCurBox::SetScrollOffset(float NewScrollOffset)
{
	DesiredScrollOffset = NewScrollOffset;
	if (MySphereCurBox.IsValid())
	{
		MySphereCurBox->SetScrollOffset(NewScrollOffset);
	}
}

void USphereCurBox::ScrollToStart()
{
	if (MySphereCurBox.IsValid())
	{
		MySphereCurBox->ScrollToStart();
	}
}

void USphereCurBox::ScrollToEnd()
{
	if (MySphereCurBox.IsValid())
	{
		MySphereCurBox->ScrollToEnd();
	}
}

USphereCurBoxSlot * USphereCurBox::AddChildToSphereCurBox(UWidget * Content)
{
	return  Cast<USphereCurBoxSlot>(Super::AddChild(Content));
}

void USphereCurBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	MySphereCurBox->SetScrollOffset(DesiredScrollOffset);
	MySphereCurBox->SetSpherePosAlignment(SpherePosAlignment);
	MySphereCurBox->SetScrollBarVisibility(UWidget::ConvertSerializedVisibilityToRuntime(ScrollBarVisibility));
	MySphereCurBox->SetScrollBarThickness(ScrollbarThickness);
	MySphereCurBox->SetScrollBarAlwaysVisible(bAlwaysShowScrollbar);
	MySphereCurBox->SetChildrenAlignment(ChildrenAlignment);
	MySphereCurBox->SetAllowOverscroll(bAllowOverscroll ? EAllowOverscroll::Yes : EAllowOverscroll::No);

}

void USphereCurBox::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MySphereCurBox.Reset();
}

void USphereCurBox::PostLoad()
{
	Super::PostLoad();

	if (GetLinkerUE4Version() < VER_UE4_DEPRECATE_UMG_STYLE_ASSETS)
	{
		if (Style_DEPRECATED != nullptr)
		{
			const FScrollBoxStyle* StylePtr = Style_DEPRECATED->GetStyle<FScrollBoxStyle>();
			if (StylePtr != nullptr)
			{
				WidgetStyle = *StylePtr;
			}

			Style_DEPRECATED = nullptr;
		}

		if (BarStyle_DEPRECATED != nullptr)
		{
			const FScrollBarStyle* StylePtr = BarStyle_DEPRECATED->GetStyle<FScrollBarStyle>();
			if (StylePtr != nullptr)
			{
				WidgetBarStyle = *StylePtr;
			}

			BarStyle_DEPRECATED = nullptr;
		}
	}
}

#if WITH_EDITOR
const FText USphereCurBox::GetPaletteCategory()
{
	return LOCTEXT("Monocular", "Monocular");
}
#endif

UClass * USphereCurBox::GetSlotClass() const
{
	return USphereCurBoxSlot::StaticClass();
}

void USphereCurBox::OnSlotAdded(UPanelSlot * Slot)
{
	if (MySphereCurBox.IsValid())
	{
		CastChecked<USphereCurBoxSlot>(Slot)->BuildSlot(MySphereCurBox.ToSharedRef());
	}
}


void USphereCurBox::OnSlotRemoved(UPanelSlot * Slot)
{
	if (MySphereCurBox.IsValid())
	{
		TSharedPtr<SWidget> Widget = Slot->Content->GetCachedWidget();
		if (Widget.IsValid())
		{
			MySphereCurBox->RemoveSlot(Widget.ToSharedRef());
		}
	}
}

void USphereCurBox::SlateHandleUserScrolled(float CurrentOffset)
{
	OnUserScrolled.Broadcast(CurrentOffset);
}

void USphereCurBox::SlateHandleUserAlignedScrolled(TSharedPtr<SWidget> Content)
{
	for (auto &Item : GetAllChildren())
	{
		if (Item->GetCachedWidget() == Content)
		{
			OnUserAlignedScrolled.Broadcast(Item);
			break;
		}
	}
}

void USphereCurBox::SlateHandleCenterAlignedScrolled(TSharedPtr<SWidget> Content)
{
	for (auto &Item : GetAllChildren())
	{
		if (Item->GetCachedWidget() == Content)
		{
			OnCenterAlignedScrolled.Broadcast(Item);
			break;
		}
	}
}

TSharedRef<SWidget> USphereCurBox::RebuildWidget()
{
	MySphereCurBox = SNew(SSphereCurBox)
		.Style(&WidgetStyle)
		.ScrollBarStyle(&WidgetBarStyle)
		.bLoopChildren(bLoopChildren)
		.ShadowFadeDistance(10.f)
		.SphereRadius(SphereRadius)
		.WidgetIntervalPos(WidgetIntervalPos)
		.bAlignedWidget(bAlignedWidget)
		.MonuseScrollKey(MouseButtonScrollKey)
		.OnUserScrolled(BIND_UOBJECT_DELEGATE(FOnUserScrolled, SlateHandleUserScrolled))
		.OnAlignedScrolled(BIND_UOBJECT_DELEGATE(FOnAlignedScrolled, SlateHandleUserAlignedScrolled))
		.OnCenterAlignedScrolled(BIND_UOBJECT_DELEGATE(FOnAlignedScrolled, SlateHandleCenterAlignedScrolled));

	for (UPanelSlot* PanelSlot : Slots)
	{
		if (USphereCurBoxSlot* TypedSlot = Cast<USphereCurBoxSlot>(PanelSlot))
		{
			TypedSlot->Parent = this;
			TypedSlot->BuildSlot(MySphereCurBox.ToSharedRef());
		}
	}

	return MySphereCurBox.ToSharedRef();
}

#undef LOCTEXT_NAMESPACE