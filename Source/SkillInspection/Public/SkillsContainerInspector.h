// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UAbstractSkill;
class FSkillPropertiesInspector;
class USkillsContainerComponent;

/** \brief This class is friend of USkillsContainerComponent. Files who have visibility of this class are allowed to peek at USkillsContainerComponent's
 * properties. The reasons this class exists are:
 * 1. To give AI knowledge on all the possible skills at its disposal, so that it can take decisions on what to do.
 * 2. To have the UI w all the possible skills at its disposal, so that the player can take decisions on what to do.
 *
 * IMPORTANT: THIS CLASS SHALL BE HIDDEN TO EVERY FILE THAT IS NOT AI OR UI RELATED.
 */
class SKILLINSPECTION_API FSkillsContainerInspector {
public:
    FSkillsContainerInspector(const TObjectPtr<USkillsContainerComponent>& skillsContainer);

    TOptional<FSkillPropertiesInspector> GetSkillPropertiesByIndex(const int32 skillIndex) const;

    TOptional<int32> GetIndexOfSkill(const TSubclassOf<UAbstractSkill>& skillClass) const;

    void ForEachSkill(TFunction<void(const FSkillPropertiesInspector&)> skillPropertiesFunc) const;

private:
    TWeakObjectPtr<USkillsContainerComponent> _skillsContainer = nullptr;
};
