set -e

[[ -z "${ARCH}" ]] && exit 2
[[ -z "${FLAVOR}" ]] && exit 3

output_file="${TRAVIS_BUILD_DIR}/cog_${ARCH}_${FLAVOR}.tar.gz"

export COGVREV="$(git describe --tags --always)"
export COGVDATE="$(git show -s --format=%ci HEAD)"
export COGVURL="$(git config --get remote.origin.url)"
export COGVOPTS="-DCOGVREV=\"${COGVREV}\" -DCOGVDATE=\"${COGVDATE// /\ }\" -DCOGVURL=\"${COGVURL//\//\\\/}\""

case "$(uname -s)" in
  "Linux")
    build_directory="./build.${ARCH}/${FLAVOR}/build"

    [[ ! -d "${build_directory}" ]] && exit 10

    pushd "${build_directory}"
    echo n | ./mvm
    # cat config.log
    popd 

    tar czf "${output_file}" "./products"
    ;;
  "Darwin")
    build_directory="./build.${ARCH}/${FLAVOR}"

    [[ ! -d "${build_directory}" ]] && exit 50

    pushd "${build_directory}"
    ./mvm
    tar czf "${output_file}" ./Cocoa*.app
    popd 
    ;;
  *)
    echo "Unsupported platform '${os_name}'." 1>&2
    exit 99
    ;;
esac

target_url="https://www.hpi.uni-potsdam.de/hirschfeld/artefacts/cog"
if [[ -n "${TRAVIS_TAG}" ]]; then
    target_url="${target_url}/${TRAVIS_TAG}"
    curl -u "${DEPLOY_CREDENTIALS}" -X MKCOL "${target_url}"
fi

curl -T "cog_${ARCH}_${FLAVOR}.tar.gz" -u "${DEPLOY_CREDENTIALS}" "${target_url}/"