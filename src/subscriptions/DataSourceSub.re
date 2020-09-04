type block_t = {timestamp: MomentRe.Moment.t};
type transaction_t = {block: block_t};
type internal_t = {
  id: ID.DataSource.t,
  owner: Address.t,
  name: string,
  description: string,
  executable: JsBuffer.t,
  transaction: option(transaction_t),
};

type t = {
  id: ID.DataSource.t,
  owner: Address.t,
  name: string,
  description: string,
  executable: JsBuffer.t,
  timestamp: MomentRe.Moment.t,
};

let toExternal = ({id, owner, name, description, executable, transaction}) => {
  id,
  owner,
  name,
  description,
  executable,
  timestamp:
    switch (transaction) {
    | Some({block}) => block.timestamp
    // TODO: Please revisit again.
    | _ => MomentRe.momentNow()
    },
};

module MultiConfig = [%graphql
  {|
  subscription DataSources($limit: Int!, $offset: Int!) {
    data_sources(limit: $limit, offset: $offset, order_by: {transaction: {block: {timestamp: desc}}, id: desc}) @bsRecord {
      id @bsDecoder(fn: "ID.DataSource.fromInt")
      owner @bsDecoder(fn: "Address.fromBech32")
      name
      description
      executable @bsDecoder(fn: "GraphQLParser.buffer")
      transaction @bsRecord {
        block @bsRecord {
          timestamp @bsDecoder(fn: "GraphQLParser.timestamp")
        }
      }
    }
  }
|}
];

module SingleConfig = [%graphql
  {|
  subscription DataSource($id: Int!) {
    data_sources_by_pk(id: $id) @bsRecord {
      id @bsDecoder(fn: "ID.DataSource.fromInt")
      owner @bsDecoder(fn: "Address.fromBech32")
      name
      description
      executable @bsDecoder(fn: "GraphQLParser.buffer")
      transaction @bsRecord {
        block @bsRecord {
          timestamp @bsDecoder(fn: "GraphQLParser.timestamp")
        }
      }
    }
  },
|}
];

module DataSourcesCountConfig = [%graphql
  {|
  subscription DataSourcesCount {
    data_sources_aggregate{
      aggregate{
        count @bsDecoder(fn: "Belt_Option.getExn")
      }
    }
  }
|}
];

let get = id => {
  let ID.DataSource.ID(id_) = id;
  let (result, _) =
    ApolloHooks.useSubscription(
      SingleConfig.definition,
      ~variables=SingleConfig.makeVariables(~id=id_, ()),
    );
  let%Sub x = result;
  switch (x##data_sources_by_pk) {
  | Some(data) => Sub.resolve(data |> toExternal)
  | None => NoData
  };
};

let getList = (~page, ~pageSize, ()) => {
  let offset = (page - 1) * pageSize;
  let (result, _) =
    ApolloHooks.useSubscription(
      MultiConfig.definition,
      ~variables=MultiConfig.makeVariables(~limit=pageSize, ~offset, ()),
    );
  result |> Sub.map(_, x => x##data_sources->Belt_Array.map(toExternal));
};

let count = () => {
  let (result, _) = ApolloHooks.useSubscription(DataSourcesCountConfig.definition);
  result
  |> Sub.map(_, x => x##data_sources_aggregate##aggregate |> Belt_Option.getExn |> (y => y##count));
};
