module Styles = {
  open Css;

  let tableLowerContainer = style([padding(`px(10))]);

  let hFlex = style([display(`flex)]);

  let alignRight = style([display(`flex), justifyContent(`flexEnd)]);
};

let renderBody = (delegation: DelegationSub.stake_t) => {
  <TBody key={delegation.operatorAddress |> Address.toBech32} minHeight=50>
    <Row>
      <Col> <HSpacing size=Spacing.lg /> </Col>
      <Col size=0.9>
        <div className=Styles.hFlex>
          <ValidatorMonikerLink
            validatorAddress={delegation.operatorAddress}
            moniker={delegation.moniker}
            identity={delegation.identity}
            width={`px(300)}
          />
        </div>
      </Col>
      <Col size=0.6>
        <div className=Styles.alignRight>
          <Text
            value={delegation.amount |> Coin.getBandAmountFromCoin |> Format.fPretty}
            code=true
          />
        </div>
      </Col>
      <Col size=0.6>
        <div className=Styles.alignRight>
          <Text
            value={delegation.reward |> Coin.getBandAmountFromCoin |> Format.fPretty}
            code=true
          />
        </div>
      </Col>
      <Col> <HSpacing size=Spacing.lg /> </Col>
    </Row>
  </TBody>;
};

let renderBodyMobile =
    ({operatorAddress, moniker, identity, amount, reward}: DelegationSub.stake_t) => {
  <MobileCard
    values=InfoMobileCard.[
      ("VALIDATOR", Validator(operatorAddress, moniker, identity)),
      ("AMOUNT\n(BAND)", Coin({value: [amount], hasDenom: false})),
      ("REWARD\n(BAND)", Coin({value: [reward], hasDenom: false})),
    ]
    key={operatorAddress |> Address.toHex}
    idx={operatorAddress |> Address.toHex}
  />;
};

[@react.component]
let make = (~address) =>
  {
    let isMobile = Media.isMobile();
    let (page, setPage) = React.useState(_ => 1);
    let pageSize = 10;
    let delegationsCountSub = DelegationSub.getStakeCountByDelegator(address);
    let delegationsSub = DelegationSub.getStakeList(address, ~pageSize, ~page, ());

    let%Sub delegationsCount = delegationsCountSub;
    let%Sub delegations = delegationsSub;

    let pageCount = Page.getPageCount(delegationsCount, pageSize);

    <div className=Styles.tableLowerContainer>
      <VSpacing size=Spacing.md />
      <div className=Styles.hFlex>
        <HSpacing size=Spacing.lg />
        <Text value={delegationsCount |> string_of_int} weight=Text.Semibold />
        <HSpacing size=Spacing.xs />
        <Text value="Validators Delegated" />
      </div>
      <VSpacing size=Spacing.lg />
      <>
        {isMobile
           ? React.null
           : <THead>
               <Row>
                 <Col> <HSpacing size=Spacing.lg /> </Col>
                 <Col size=0.9>
                   <Text
                     block=true
                     value="VALIDATOR"
                     size=Text.Sm
                     weight=Text.Bold
                     spacing={Text.Em(0.05)}
                     color=Colors.gray6
                   />
                 </Col>
                 <Col size=0.6>
                   <div className=Styles.alignRight>
                     <Text
                       block=true
                       value="AMOUNT (BAND)"
                       size=Text.Sm
                       weight=Text.Bold
                       spacing={Text.Em(0.05)}
                       color=Colors.gray6
                     />
                   </div>
                 </Col>
                 <Col size=0.6>
                   <div className=Styles.alignRight>
                     <Text
                       block=true
                       value="REWARD (BAND)"
                       size=Text.Sm
                       spacing={Text.Em(0.05)}
                       weight=Text.Bold
                       color=Colors.gray6
                     />
                   </div>
                 </Col>
                 <Col> <HSpacing size=Spacing.lg /> </Col>
               </Row>
             </THead>}
        {delegations
         ->Belt.Array.map(delegation => {
             isMobile ? renderBodyMobile(delegation) : renderBody(delegation)
           })
         ->React.array}
        <VSpacing size=Spacing.lg />
        <Pagination currentPage=page pageCount onPageChange={newPage => setPage(_ => newPage)} />
      </>
    </div>
    |> Sub.resolve;
  }
  |> Sub.default(_, React.null);
