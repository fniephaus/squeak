[[ -z "${CARCH}" ]] && exit 2
[[ -z "${CFLAV}" ]] && exit 3

output_file="${TRAVIS_BUILD_DIR}/cog_${CARCH}_${CFLAV}.tar.gz"

export COGVREV="$(git describe --tags --always)"
export COGVDATE="$(echo $(git show -s --format=%ci HEAD))"
export COGVURL="$(git config --get remote.origin.url)"
export COGVOPTS=$'-DCOGVREV="${COGVREV}" -DCOGVDATE="${COGVDATE}" -DCOGVURL="${COGVURL}"'

case "$(uname -s)" in
  "Linux")
    build_directory="./build.${CARCH}/${CFLAV}/build"

    [[ ! -d "${build_directory}" ]] && exit 10

    pushd "${build_directory}"
    echo n | ./mvm
    cat config.log
    popd 

    tar czf "${output_file}" "./products"
    ;;
  "Darwin")
    build_directory="./build.${CARCH}/${CFLAV}"

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

