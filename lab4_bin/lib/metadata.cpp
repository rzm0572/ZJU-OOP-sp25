#include "metadata.h"

MetadataList::MetadataList() {
    metadata_list.clear();
}

void MetadataList::add_metadata(const Metadata& metadata) {
    metadata_list.push_back(metadata);
}
