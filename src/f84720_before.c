void DataReductionProxyConfig::UpdateConfigForTesting(
    bool enabled,
    bool secure_proxies_allowed,
    bool insecure_proxies_allowed) {
  enabled_by_user_ = enabled;
  network_properties_manager_->ResetWarmupURLFetchMetrics();
  network_properties_manager_->SetIsSecureProxyDisallowedByCarrier(
      !secure_proxies_allowed);
  if (!insecure_proxies_allowed !=
          network_properties_manager_->HasWarmupURLProbeFailed(
              false /* secure_proxy */, true /* is_core_proxy */)) {
    network_properties_manager_->SetHasWarmupURLProbeFailed(
        false /* secure_proxy */, true /* is_core_proxy */,
        !insecure_proxies_allowed);
  }
}
