#pragma once

#include "CoreMinimal.h"
#include "Containers/Array.h"
#include "Misc/TextFilter.h"
#include "Templates/SharedPointer.h"

//
class Searcher
{
public:

	template<typename T>
	static void Filter(const TSharedPtr<TTextFilter<T>>& F, const FText& Text, const TArray<T>& As, TArray<T>& Bs)
	{
        check(F.IsValid()); // Must be a valid pointer.

        // Feed the raw text from the UI directly to the filter
        F->SetRawFilterText(Text);

        Bs.Empty();

        for (const T& A : As)
        {
            // The filter handles all the complex tokenization, AND, OR, and NOT logic!
            if (F->PassesFilter(A))
            {
                Bs.Add(A);
            }
        }
	}
};
