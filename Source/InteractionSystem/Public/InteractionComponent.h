// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractorInterface.h"
#include "Components/WidgetComponent.h"
#include "UObject/WeakInterfacePtr.h"

#include "InteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginFocus, TScriptInterface<IInteractorInterface>, interactor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndFocus, TScriptInterface<IInteractorInterface>, interactor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginInteraction, TScriptInterface<IInteractorInterface>, interactor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndInteraction, TScriptInterface<IInteractorInterface>, interactor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteraction, TScriptInterface<IInteractorInterface>, interactor);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INTERACTIONSYSTEM_API UInteractionComponent : public UWidgetComponent {
     GENERATED_BODY()
     
public:
	UInteractionComponent();

    /** \brief Shows the associated widget and communicates to the owner actor that focus has begun */
    void BeginFocus(TScriptInterface<IInteractorInterface> interactor);
    /** \brief Hides the associated widget and communicates to the owner actor that focus has begun */
    void EndFocus(TScriptInterface<IInteractorInterface> interactor);
    /** \brief If the interaction is possible, communicates to the owner actor that the interactor has begun the
     * interaction process. The interactor is cached. */
    void BeginInteraction(TScriptInterface<IInteractorInterface> interactor);
    /** \brief Communicates to the owner actor that interaction has ended. The interactor is removed from the internal
     * cache. */
    void EndInteraction(TScriptInterface<IInteractorInterface> interactor);
	/** \brief If the interaction is possible, communicates to the owner actor that it's time to make the actual
	 * interaction occur. */
    void Interact(TScriptInterface<IInteractorInterface> interactor);

    /** \brief Returns the time that must elapse between BeginInteraction() and Interaction() calls. It's the
     * interactor's responsibility to make sure the calls are performed properly. */
    double GetInteractionTime() const { return _interactionTime; }

	/** \brief Returns the maximum distance the owner actor can be from any potential interactor to be interactable. */
	double GetInteractionDistance() const { return _interactionDistance; }

protected:
	void BeginPlay() override;

	void Deactivate() override;

	bool _canInteract(const TScriptInterface<IInteractorInterface>& interactor) const;

	// Every AController currently interacting with this (player controller, AI controllerS)
	TSet<TScriptInterface<IInteractorInterface>> _interactors;
     
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	double _interactionTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	double _interactionDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool _bAllowMultipleInteractors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	FText _interactableNameText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	FText _interactableActionText;

	// Broadcasts when the interacting controller starts focusing the owner actor (for a player controller, when the
	// cursor is hovering the owner actor)
	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FOnBeginFocus _onBeginFocus;

	// Broadcasts when the interacting controller stops focusing the owner actor (for a player controller, when the
	// cursor stops hovering the owner actor)
	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FOnEndFocus _onEndFocus;

	// Broadcasts when the interacting controller starts interacting with the owner actor (for a player controller,
	// when the interaction key is pressed)
	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FOnBeginInteraction _onBeginInteraction;

	// Broadcasts when the interacting controller has interacted with the owner actor for the required amount of time
	// (for a player controller, when the interaction key has been held down for the required amount of time)
	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FOnInteraction _onInteraction;

	// Broadcasts when the interacting controller stops interacting with the owner actor (for a player controller, when
	// the interaction key is released)
	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FOnEndInteraction _onEndInteraction;
};
