//
// Created by Pavel Prokofyev on 2019-01-03.
//

#ifndef __USBGEN_AUDIO_SELECTOR_UNIT__
#define __USBGEN_AUDIO_SELECTOR_UNIT__

#include "Composite.h"
#include "Descriptor.h"

namespace usb {
namespace audio {

template <size_t N>
struct SelectorUnitDescriptor : public DescriptorBase {
  using BaseTy = DescriptorBase;

  uint8_t bDescriptorSubtype = {SelectorUnit};
  uint8_t bUnitID = {0};
  uint8_t bNrInPins = {0};
  std::array<uint8_t, N> baSourceID = {0};
  uint8_t iSelector = {0};

  constexpr SelectorUnitDescriptor()
      : BaseTy(sizeof(SelectorUnitDescriptor), AudioInterface) {
    static_assert(sizeof(SelectorUnitDescriptor) == 6 + N ,
                  "unexpected descriptor size");
  }
};

template <size_t N, typename... AggregatesT>
struct SelectorUnitComposite
    : public detail::CompositeDescriptor<SelectorUnitDescriptor<N>,
                                         AggregatesT...> {
  using BaseTy =
      detail::CompositeDescriptor<SelectorUnitDescriptor<N>, AggregatesT...>;

  constexpr SelectorUnitComposite(AggregatesT &&... Aggs)
      : BaseTy(std::move(Aggs)...) {}
};

template <size_t N> struct SelectorUnitHelper {
  template <typename... AggregateT>
  using type = SelectorUnitComposite<N, AggregateT...>;
};

template <size_t N, typename... AggregateT>
static constexpr auto make_selector_unit(AggregateT &&... Aggs) {
  return detail::composite<SelectorUnitHelper<N>::template type>(std::move(Aggs)...);
}

namespace selector_unit {
APPLICATOR(bUnitID, uint8_t, bUnitID)
APPLICATOR(bNrInPins, uint8_t, bNrInPins)
APPLICATOR(iSelector, uint8_t, iSelector)

template <uint8_t... Vals> struct baSourceID : detail::ApplicatorBase {
  constexpr baSourceID() {}

  template <typename T> constexpr void apply(T &Object) const {
    Object.baSourceID = {Vals...};
  }
};

} // namespace feature_unit

} // namespace audio
} // namespace usb

#endif //__USBGEN_AUDIO_FEATURE_UNIT__