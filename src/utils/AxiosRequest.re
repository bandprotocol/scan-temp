[@bs.deriving abstract]
type t = {
  executable: string,
  calldata: string,
  timeout: int,
};

/* TODO: FIX THIS MESS */
let convert: t => Js.t('a) = [%bs.raw {|
function(data) {
  return {...data};
}
  |}];

let request = (data: t) => {
  let%Promise response =
    Axios.postData("https://d3n.bandprotocol.com/bandsv/request", convert(data));
  Promise.ret(response);
};

let execute = (data: t) => {
  let%Promise response = Axios.postData(Env.lambda, convert(data));
  Promise.ret(response);
};
